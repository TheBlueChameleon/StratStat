#include <string>
using namespace std::string_literals;

#include <spdlog/spdlog.h>

#include "engine.hpp"
#include "interface.hpp"
#include "shared.hpp"

void initPkmnDb(const std::filesystem::path& pkmnDefs)
{
    spdlog::trace("INITIALIZING PKMNDB");

    auto& engine = Engine::getInstance();

    auto headerRequirements = std::vector<VariantContentInfo>();
    getPkmnDefHeaders(headerRequirements);
    const auto& pkmnIdentifierKey = headerRequirements.front().identifier;

    auto csv = DefaultCsvReader();
    csv.mmap(pkmnDefs.c_str());

    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, pkmnDefs.c_str());
    const auto expectedColumnCount = columnData.size();

    for (const auto& row: csv)
    {
        const auto pkmnData = parseCsvRow(row, columnData);
        if (pkmnData.size() == 0)
        {
            continue;
        }
        else if (pkmnData.size() < expectedColumnCount)
        {
            spdlog::error(
                "  MALFORMED PKMN DEFINITION FOR '{}' (IGNORED)",
                std::get<static_cast<int>(VariantContentID::Text)>(pkmnData.at(pkmnIdentifierKey)),
                pkmnData.size(),
                expectedColumnCount
            );
        }
        else
        {
            spdlog::trace(
                "  PUT SPECIES '{}' IN DB",
                std::get<static_cast<int>(VariantContentID::Text)>(pkmnData.at(pkmnIdentifierKey))
            );

            engine.getPkmnDbMutable().add(CommonValueCollection(pkmnIdentifierKey, pkmnData));

            //engine.getPkmnDbMutable().add(BasePkmnDef(pkmnIdentifierKey, pkmnData));
        }
    }

    spdlog::trace("  KNOWN SPECIES: {}", engine.getPkmnDb().size());
    spdlog::trace("... SUCCESS");
}

void initMoveDb(const std::filesystem::path& moveDefs)
{
    spdlog::trace("INITIALIZING MOVEDB FROM {}", moveDefs.c_str());

    auto& engine = Engine::getInstance();

    auto headerRequirements = std::vector<VariantContentInfo>();
    getMoveDefHeaders(headerRequirements);
    const auto& moveIdentifierKey = headerRequirements.front().identifier;

    auto csv = DefaultCsvReader();
    csv.mmap(moveDefs.c_str());

    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, moveDefs.c_str());

    for (const auto& row: csv)
    {
        const auto moveData = parseCsvRow(row, columnData);
        if (moveData.size() == 0)
        {
            continue;
        }
        spdlog::trace(
            "  PUT MOVE '{}' IN DB",
            std::get<static_cast<int>(VariantContentID::Text)>(moveData.at(moveIdentifierKey))
        );

        engine.getMoveDbMutable().add(CommonValueCollection(moveIdentifierKey, moveData));
    }

    spdlog::trace("  KNOWN MOVES: {}", engine.getMoveDb().size());
    spdlog::trace("... SUCCESS");
}

std::vector<CsvMappingInfo> analyzeHeader(const DefaultCsvReader::Row& header, const std::vector<VariantContentInfo>& headerRequirements, const std::string& filename)
{
    std::vector<CsvMappingInfo> result;

    std::vector<std::string> presentHeaders;

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
            spdlog::critical("REQUIRED COLUMN '{}' IN {} NOT FOUND", requiredColumn, filename);
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
