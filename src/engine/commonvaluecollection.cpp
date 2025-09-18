#include <iostream>

#include "commonvaluecollection.hpp"

CommonValueCollection::CommonValueCollection(const std::string& identifierKey, const CommonValueMap &data) :
    identifierKey(identifierKey),
    data(data)
{}

const std::string& CommonValueCollection::getIdentifier() const
{
    const VariantContentType& lookup = data.at(identifierKey);
    const auto contentType = static_cast<int>(VariantContentID::Text);
    return std::get<contentType>(lookup);
}

const VariantContentType& CommonValueCollection::get(std::string key) const
{
    return data.at(key);
}
