#include <string>
using namespace std::string_literals;

#include <list>

#include <spdlog/spdlog.h>

#include "engine.hpp"
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
    initDb(pkmnDefs, headerRequirements, Engine::getInstance().getPkmnDbMutable());

    spdlog::trace("  KNOWN SPECIES: {}", Engine::getInstance().getPkmnDb().size());
    spdlog::trace("... SUCCESS");
}

void initMoveDb(const std::filesystem::path& moveDefs)
{
    spdlog::trace("INITIALIZING MOVEDB FROM {}", moveDefs.c_str());

    auto headerRequirements = std::vector<VariantContentInfo>();
    getMoveDefHeaders(headerRequirements);
    initDb(moveDefs, headerRequirements, Engine::getInstance().getMoveDbMutable());

    spdlog::trace("  KNOWN MOVES: {}", Engine::getInstance().getMoveDb().size());
    spdlog::trace("... SUCCESS");
}

void initDb(
    const std::filesystem::path& filename,
    const std::vector<VariantContentInfo>& headerRequirements,
    CommonDatabase& db
)
{
    auto csv = getCsvData(filename);

    const auto& identifierKey = headerRequirements.front().identifier;
    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, filename);
    const auto expectedColumnCount = columnData.size();

    for (const auto& row: csv)
    {
        const auto dbEntry = parseCsvRow(row, columnData);
        if (dbEntry.size() == 0)
        {
            continue;
        }
        else if (dbEntry.size() < expectedColumnCount)
        {
            spdlog::error(
                "  MALFORMED DEFINITION FOR '{}' (IGNORED)",
                std::get<static_cast<int>(VariantContentID::Text)>(dbEntry.at(identifierKey)),
                dbEntry.size(),
                expectedColumnCount
            );
        }
        else
        {
            spdlog::trace(
                "  PUT SPECIES '{}' IN DB",
                std::get<static_cast<int>(VariantContentID::Text)>(dbEntry.at(identifierKey))
            );

            db.add(CommonValueCollection(identifierKey, dbEntry));
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

std::unordered_map<std::string, VariantContentType> parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData)
{
    std::unordered_map<std::string, VariantContentType> result;

    auto nextRelevant = columnData.begin();
    for (long i = -1; const auto& cell : row)
    {
        ++i;
        if (i == nextRelevant->columnID)
        {
            std::string buffer;
            cell.read_value(buffer);

            result[nextRelevant->columnName] = variantFromString(buffer, nextRelevant->contentID);

            ++nextRelevant;
        }
        else
        {
            continue;
        }
    }

    return result;
}

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
