#ifndef SHARED_HPP
#define SHARED_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <csv2/reader.hpp>
#include <jsonxx.h>

#include "types.hpp"

void initPkmnDb(const std::filesystem::path& pkmnDefs);
void initMoveDb(const std::filesystem::path& pkmnDefs);

std::vector<CsvMappingInfo> analyzeHeader(const DefaultCsvReader::Row& header, const std::vector<VariantContentInfo>& headerRequirements, const std::string& filename);
std::unordered_map<std::string, VariantContentType> parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData);

VariantContentType variantFromString(const std::string& input, const VariantContentID contentID);

#endif // SHARED_HPP
