#include <iostream>

#include "variantdef.hpp"

VariantDef::VariantDef(const std::string& identifierKey, const std::unordered_map<std::string, VariantContentType>& data) :
    identifierKey(identifierKey),
    data(data)
{}

const std::string& VariantDef::getIdentifier() const
{
    const VariantContentType& lookup = data.at(identifierKey);
    const auto contentType = static_cast<int>(VariantContentID::Text);
    return std::get<contentType>(lookup);
}

const VariantContentType& VariantDef::get(std::string key) const
{
    return data.at(key);
}
