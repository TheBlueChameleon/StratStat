#ifndef SHAREDTYPES_H
#define SHAREDTYPES_H

#include <list>
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
    using DefaultCsvReader = csv2::Reader<
                             csv2::delimiter<','>,
                             csv2::quote_character<'"'>,
                             csv2::first_row_is_header<true>,
                             csv2::trim_policy::trim_whitespace
                             >;

    /* ---------------------------------------------------------------------- */
    // common variant definitions

    class VariantContentType : public std::variant<int, double, std::string, std::monostate>
    {
        public:
            using std::variant<int, double, std::string, std::monostate>::variant;

            bool hasValue() const;

            static VariantContentType error();
    };

    enum class VariantContentID {Integer, Double, Text, Error};

    struct VariantContentInfo
    {
        std::string      identifier;
        VariantContentID contentID;
    };

    std::string getVariantContentIDName(const VariantContentID id);

    /* ---------------------------------------------------------------------- */
    // common variant containers

    // TODO: Make proper classes

    using CommonValueMap = std::unordered_map<std::string, VariantContentType>;
    using CommonValueMapVector = std::vector<CommonValueMap>;

    class CommonValueMapValidationResult;
    using CommonValueMapValidator = CommonValueMapValidationResult(*)(const CommonValueMap& dbEntry);

    /* ---------------------------------------------------------------------- */

    struct CsvMappingInfo
    {
        long             columnID;
        std::string      columnName;
        VariantContentID contentID;
    };

    class AllowedValues : public std::unordered_set<std::string>
    {
        public:
            using std::unordered_set<std::string>::unordered_set;
    };

    class ErrorMessgeList : public std::list<std::string>
    {
        public:
            using std::list<std::string>::list;
    };
}

#endif // SHAREDTYPES_H
