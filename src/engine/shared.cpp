#include <fstream>
#include <list>
#include <string>
using namespace std::string_literals;

#include <spdlog/spdlog.h>

#include "engine.hpp"
#include "innerinterface.hpp"
#include "interface.hpp"
#include "shared.hpp"

bool readyFlag = false;

DefaultCsvReader getCsvData(const std::filesystem::path& csvFileName)
{
    auto csv = DefaultCsvReader();
    csv.mmap(csvFileName.c_str());
    return csv;
}

void initPkmnDb(const std::filesystem::path& pkmnDefs)
{
    spdlog::trace("INITIALIZING PKMNDB");

    auto headerRequirements = std::vector<VariantContentInfo>();
    getPkmnDefHeaders(headerRequirements);
    initDb(pkmnDefs, headerRequirements, validatePkmnDef, Engine::getInstance().getPkmnDbMutable());

    spdlog::trace("  KNOWN SPECIES: {}", Engine::getInstance().getPkmnDb().size());
    spdlog::trace("... SUCCESS");
}

void initMoveDb(const std::filesystem::path& moveDefs)
{
    spdlog::trace("INITIALIZING MOVEDB FROM {}", moveDefs.c_str());

    auto headerRequirements = std::vector<VariantContentInfo>();
    getMoveDefHeaders(headerRequirements);
    initDb(moveDefs, headerRequirements, validateMoveDef, Engine::getInstance().getMoveDbMutable());

    spdlog::trace("  KNOWN MOVES: {}", Engine::getInstance().getMoveDb().size());
    spdlog::trace("... SUCCESS");
}

void initDb(
    const std::filesystem::path& filename,
    const std::vector<VariantContentInfo>& headerRequirements,
    CommonValueMapValidator validator,
    CommonDatabase& db
)
{
    const auto csv = getCsvData(filename);
    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, filename);

    for (const auto& row: csv)
    {
        const auto dbProtoEntry = parseCsvRow(row, columnData);
        if (dbProtoEntry.size() == 0)
        {
            continue;
        }

        const auto dbEntry = validator(dbProtoEntry);
        if (dbEntry.has_value())
        {
            const auto& dbValue = dbEntry.value();
            spdlog::trace("  PUT '{}' IN DB", dbValue.getIdentifier());
            db.add(dbValue);
        }
        else
        {
            std::string rowBuffer;
            row.read_raw_value(rowBuffer);
            spdlog::error("  MALFORMED LINE: '{}' (IGNORED)", rowBuffer);
        }
    }
}


std::vector<CsvMappingInfo> analyzeHeader(
    const DefaultCsvReader::Row& header,
    const std::vector<VariantContentInfo>& headerRequirements,
    const std::string& filename
)
{
    std::vector<CsvMappingInfo> result;

    std::list<std::string> presentHeaders;
    for (const auto& cell : header)
    {
        std::string cellContent;
        cell.read_value(cellContent);
        presentHeaders.push_back(cellContent);
    }

    const auto start = presentHeaders.begin();
    const auto notAvailable = presentHeaders.end();
    for (const auto& requirement: headerRequirements)
    {
        const std::string& requiredColumn = requirement.identifier;
        const auto contentID = requirement.contentID;

        const auto it = std::ranges::find(presentHeaders, requiredColumn);
        if (it == notAvailable)
        {
            spdlog::critical("IN FILE {}: REQUIRED COLUMN '{}' NOT FOUND", filename, requiredColumn);
            std::exit(-1);
        }
        else
        {
            const auto colID = std::distance(start, it);
            result.push_back({colID, requiredColumn, contentID});
            spdlog::trace("  COLUMN {} IDENTIFIED AS '{}'", colID, requiredColumn);
        }
    }

    std::sort(result.begin(), result.end(), [](const CsvMappingInfo& left, const CsvMappingInfo& right)
    {
        return left.columnID < right.columnID;
    });

    return result;
}

CommonValueMap parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData)
{
    std::unordered_map<std::string, VariantContentType> result;

    auto nextRelevant = columnData.begin();
    for (long i = -1; const auto& cell : row)
    {
        ++i;
        if (i != nextRelevant->columnID)
        {
            continue;
        }

        std::string buffer;
        cell.read_value(buffer);
        result[nextRelevant->columnName] = variantFromString(buffer, nextRelevant->contentID);
        ++nextRelevant;
    }

    return result;
}

// TODO error resistance
VariantContentType variantFromString(const std::string& input, const VariantContentID contentID)
{
    switch (contentID)
    {
        case VariantContentID::Integer:
            return std::stoi(input);
        case VariantContentID::Double:
            return std::stod(input);
        case VariantContentID::Text:
            return input;
    }
    throw std::runtime_error("Unknown contentID: "s + std::to_string(static_cast<int>(contentID)));
}

void loadTeam1(const std::filesystem::path& teamDef)
{
    spdlog::trace("  READING TEAM 1 FROM {}", teamDef.c_str());
    auto& engine = Engine::getInstance();
    loadTeam(teamDef, engine.getPlayer1Mutable(), engine.getTeam1Mutable());
    spdlog::trace("  ... OK");
}

void loadTeam2(const std::filesystem::path& teamDef)
{
    spdlog::trace("  READING TEAM 2 FROM {}", teamDef.c_str());
    auto& engine = Engine::getInstance();
    loadTeam(teamDef, engine.getPlayer2Mutable(), engine.getTeam2Mutable());
    spdlog::trace("  ... OK");
}

void loadTeam(const std::filesystem::path& fileName, CommonValueMap& playerDef, CommonValueMapVector& teamDef)
{
    auto file = std::ifstream(fileName);
    jsonxx::Object json;
    const bool success = json.parse(file);
    if (!success)
    {
        spdlog::critical("The team definition file '{}' is not a valid JSON file.", fileName.c_str());
        std::exit(-1);
    }

    JsonValidation::SpecificationSet specs;
    getTeamDefStructure(specs);

    const auto validationResult = JsonValidation::validate(json, specs);

    if (validationResult.hasMessages())
    {
        spdlog::warn("In team definition file '{}':", fileName.c_str());
        for (const auto& warning: validationResult.getValidationWarnings())
        {
            spdlog::warn(warning);
        }
    }

    if (!validationResult.isValid())
    {
        spdlog::critical("In team definition file '{}':", fileName.c_str());
        for (const auto& error: validationResult.getValidationErrors())
        {
            spdlog::critical(error);
        }
        std::exit(-1);
    }

    getValidatedTeamDef(json);
}
