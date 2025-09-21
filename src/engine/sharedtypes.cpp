#include <iostream>

#include "sharedtypes.hpp"

namespace StratStat
{
    bool VariantContentType::hasValue() const
    {
        return this->index() != static_cast<int>(VariantContentID::Error);
    }

    VariantContentType VariantContentType::error()
    {
        return VariantContentType(std::monostate());
    }

    std::string getVariantContentIDName(const VariantContentID id)
    {
        switch (id)
        {
            case VariantContentID::Integer:
                return "integer";
            case VariantContentID::Double:
                return "decimal number";
            case VariantContentID::Text:
                return "text";
            case VariantContentID::Error:
                return "error";
            default:
                throw std::runtime_error("Unknown Content ID");
        }
    }
}
