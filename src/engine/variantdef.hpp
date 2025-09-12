#ifndef VARIANTDEF_HPP
#define VARIANTDEF_HPP

#include <string>
#include <unordered_map>
#include <variant>

#include "types.hpp"

class VariantDef
{
    private:
        const std::string identifierKey;
        std::unordered_map<std::string, VariantContentType> data;

    public:
        VariantDef(const std::string& identifierKey, const std::unordered_map<std::string, VariantContentType>& data);

        const std::string& getIdentifier() const;
};

#endif // VARIANTDEF_HPP
