#include "variantdef.hpp"

VariantDef::VariantDef(const std::unordered_map<std::string, VariantContentType> &data)
{

}

const std::string& VariantDef::getIdentifier() const
{
    return identifier;
}
