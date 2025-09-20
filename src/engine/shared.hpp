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

extern bool readyFlag;

void initPkmnDb(const std::filesystem::path& pkmnDefs);
void initMoveDb(const std::filesystem::path& pkmnDefs);
void initDb(
    const std::filesystem::path& filename,
    const std::vector<VariantContentInfo>& headerRequirements,
    CommonValueMapValidator validator,
    CommonDatabase& db
);

void loadTeam1(const std::filesystem::path& teamDef);
void loadTeam2(const std::filesystem::path& teamDef);
void loadTeam(const std::filesystem::path& defFile, CommonValueMap& playerDef, CommonValueMapVector& teamDef);

std::vector<CsvMappingInfo> analyzeHeader(const DefaultCsvReader::Row& header, const std::vector<VariantContentInfo>& headerRequirements, const std::string& filename);
CommonValueMap parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData);

VariantContentType variantFromString(const std::string& input, const VariantContentID contentID);

#endif // SHARED_HPP
