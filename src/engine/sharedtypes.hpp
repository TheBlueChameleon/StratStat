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

    class VariantContentType : public std::variant<std::monostate, int, double, std::string>
    {
        public:
            static VariantContentType error();

            using std::variant<std::monostate, int, double, std::string>::variant;

            bool hasValue() const;

            const bool isInt() const;
            const bool isDouble() const;
            const bool isString() const;

            const int asInt() const;
            const double asDouble() const;
            const std::string& asString() const;

            std::string to_string() const;

    };

    enum class VariantContentID {Error, Integer, Double, Text};

    struct VariantContentInfo
    {
        std::string      identifier;
        VariantContentID contentID;
    };

    std::string getVariantContentIDName(const VariantContentID id);

    /* ---------------------------------------------------------------------- */
    // common variant containers


    class CommonValueMap : public std::unordered_map<std::string, VariantContentType>
    {
        public:
            using std::unordered_map<std::string, VariantContentType>::unordered_map;

            std::string to_string() const;
    };

    // TODO: Make proper classes
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
