#ifndef SHAREDTYPES_H
#define SHAREDTYPES_H

#include <string>
#include <utility>
#include <unordered_map>
#include <variant>

#include <csv2/reader.hpp>

enum class VariantContentID {Integer, Double, Text};
using VariantContentType = std::variant<int, double, std::string>;
struct VariantContentInfo
{
    std::string      identifier;
    VariantContentID contentID;
};

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
