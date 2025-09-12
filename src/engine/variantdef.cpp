#include "variantdef.hpp"

VariantDef::VariantDef(const std::map<std::string, VariantContentType>& data) {}

const std::string& VariantDef::getIdentifier() const
{
    return identifier;
}
