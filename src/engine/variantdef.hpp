#ifndef VARIANTDEF_HPP
#define VARIANTDEF_HPP

#include <string>
#include <unordered_map>
#include <variant>

#include "types.hpp"

class VariantDef
{
    private:
        std::string identifier;
        std::unordered_map<std::string, VariantContentType> data;

    public:
        VariantDef(const std::unordered_map<std::string, VariantContentType>& data);

        const std::string& getIdentifier() const;
};

#endif // VARIANTDEF_HPP
