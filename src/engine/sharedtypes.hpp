#ifndef SHAREDTYPES_H
#define SHAREDTYPES_H

#include <string>
#include <optional>
#include <utility>
#include <unordered_map>
#include <variant>

#include <csv2/reader.hpp>

class CommonValueCollection;

enum class VariantContentID {Integer, Double, Text};
using VariantContentType = std::variant<int, double, std::string>;
struct VariantContentInfo
{
    std::string      identifier;
    VariantContentID contentID;
};

using CommonValueMap = std::unordered_map<std::string, VariantContentType>;
using CommonValueMapVector = std::vector<CommonValueMap>;
using CommonValueMapValidator = std::optional<CommonValueCollection>(*)(const CommonValueMap& dbEntry);

using DefaultCsvReader = csv2::Reader<
                         csv2::delimiter<','>,
                         csv2::quote_character<'"'>,
                         csv2::first_row_is_header<true>,
                         csv2::trim_policy::trim_whitespace
                         >;

struct CsvMappingInfo
{
    long             columnID;
    std::string      columnName;
    VariantContentID contentID;
};


#endif // SHAREDTYPES_H
