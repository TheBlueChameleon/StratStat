#ifndef COMMONVALUECOLLECTION_H
#define COMMONVALUECOLLECTION_H

#include <string>
#include <unordered_map>
#include <variant>

#include "types.hpp"

class CommonValueCollection
{
    private:
        const std::string identifierKey;
        std::unordered_map<std::string, VariantContentType> data;

    public:
        CommonValueCollection(const std::string& identifierKey, const std::unordered_map<std::string, VariantContentType>& data);

        const std::string& getIdentifier() const;
        const VariantContentType& get(std::string key) const;
};

#endif // COMMONVALUECOLLECTION_H
