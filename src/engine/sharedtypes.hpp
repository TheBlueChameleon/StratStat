#ifndef SHAREDTYPES_H
#define SHAREDTYPES_H

#include <string>
#include <optional>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <csv2/reader.hpp>

namespace StratStat
{
    class CommonValueCollection;

    enum class VariantContentID {Integer, Double, Text};
    using VariantContentType = std::variant<int, double, std::string>;
    struct VariantContentInfo
    {
        std::string      identifier;
        VariantContentID contentID;
    };

    class CommonValueMapValidationResult;
    using AllowedValues = std::unordered_set<std::string>;
    using CommonValueMap = std::unordered_map<std::string, VariantContentType>;
    using CommonValueMapVector = std::vector<CommonValueMap>;
    using CommonValueMapValidator = CommonValueMapValidationResult(*)(const CommonValueMap& dbEntry);

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

    // bool operator==(const StratStat::VariantContentType& lhs, const StratStat::VariantContentType& rhs);
}

// namespace std
// {
//     template <> struct hash<StratStat::VariantContentType>
//     {
//         size_t operator()(const StratStat::VariantContentType& variant) const;
//     };
// }

// _Z15validatePkmnDefRKSt13unordered_mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt7variantIJidS5_EESt4hashIS5_ESt8equal_toIS5_ESaISt4pairIKS5_S7_EEE
// _Z15validatePkmnDefRKSt13unordered_mapINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESt7variantIJidS5_EESt4hashIS5_ESt8equal_toIS5_ESaISt4pairIKS5_S7_EEE
#endif // SHAREDTYPES_H
