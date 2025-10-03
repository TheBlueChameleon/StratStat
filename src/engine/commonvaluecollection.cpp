#include "commonvaluecollection.hpp"

namespace StratStat
{
    CommonValueCollection::CommonValueCollection(const std::string& identifierKey, const CommonValueMap& data) :
        identifierKey(identifierKey),
        data(data)
    {}

    const std::string& CommonValueCollection::getIdentifier() const
    {
        return data.at(identifierKey).asString();
    }

    const VariantContentType& CommonValueCollection::get(std::string key) const
    {
        return data.at(key);
    }
}
