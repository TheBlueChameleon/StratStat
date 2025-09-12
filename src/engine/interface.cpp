#include <algorithm>
#include <ranges>
#include <string>
using namespace std::string_literals;

#include "interface.hpp"

extern "C" {
    void connectLogger(const std::shared_ptr<spdlog::logger>& logger)
    {
        spdlog::set_default_logger(logger);
    }
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

std::map<std::string, VariantContentType> parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData)
{
    std::map<std::string, VariantContentType> result;

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
