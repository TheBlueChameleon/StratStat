#ifndef VARIANTDEF_HPP
#define VARIANTDEF_HPP

#include <map>
#include <string>
#include <variant>

#include "types.hpp"

class VariantDef
{
    private:
        std::string identifier;
        std::map<std::string, VariantContentType> data;

    public:
        VariantDef(const std::map<std::string, VariantContentType>& data);

        const std::string& getIdentifier() const;
};

#endif // VARIANTDEF_HPP
