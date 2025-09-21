#ifndef SHARED_HPP
#define SHARED_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <csv2/reader.hpp>
#include <jsonxx.h>

#include "commondatabase.hpp"
#include "sharedtypes.hpp"

namespace StratStat
{
    extern bool readyFlag;

    void loadPkmnDb(const std::filesystem::path& pkmnDefs);
    void loadMoveDb(const std::filesystem::path& pkmnDefs);
    void loadDb(
        const std::filesystem::path& filename,
        const std::vector<StratStat::VariantContentInfo>& headerRequirements,
        StratStat::CommonValueMapValidator validator,
        CommonDatabase& db
    );

    void loadTeam1(const std::filesystem::path& teamDef);
    void loadTeam2(const std::filesystem::path& teamDef);
    void loadTeam(
        const std::filesystem::path& defFile,
        StratStat::CommonValueMap& playerDef,
        StratStat::CommonValueMapVector& teamDef
    );

    std::vector<StratStat::CsvMappingInfo> analyzeHeader(
        const StratStat::DefaultCsvReader::Row& header,
        const std::vector<VariantContentInfo>& headerRequirements,
        const std::string& filename
    );
    CommonValueMap parseCsvRow(const StratStat::DefaultCsvReader::Row& row, const std::vector<StratStat::CsvMappingInfo>& columnData);

    VariantContentType variantFromString(const std::string& input, const StratStat::VariantContentID contentID);
}

#endif // SHARED_HPP
