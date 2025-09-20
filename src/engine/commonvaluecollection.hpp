#ifndef COMMONVALUECOLLECTION_H
#define COMMONVALUECOLLECTION_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "sharedtypes.hpp"

class CommonValueCollection
{
    private:
        const std::string identifierKey;
        CommonValueMap data;

    public:
        CommonValueCollection(const std::string& identifierKey, const CommonValueMap& data);

        const std::string& getIdentifier() const;
        const VariantContentType& get(std::string key) const;
};

#endif // COMMONVALUECOLLECTION_H
