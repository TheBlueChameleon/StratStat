#include <iostream>
#include <sstream>

#include "sharedtypes.hpp"

namespace StratStat
{
    bool VariantContentType::hasValue() const
    {
        return this->index() != static_cast<int>(VariantContentID::Error);
    }

    std::string VariantContentType::to_string() const
    {
        switch (this->index())
        {
            case static_cast<int>(VariantContentID::Integer):
                return std::to_string(std::get<int>(*this));
            case static_cast<int>(VariantContentID::Double):
                return std::to_string(std::get<double>(*this));
            case static_cast<int>(VariantContentID::Text):
                return std::get<std::string>(*this);
            case static_cast<int>(VariantContentID::Error):
                return "<error>";
        }
        throw std::runtime_error("unsupported type");
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

    std::string CommonValueMap::to_string() const
    {
        std::stringstream ss;

        for (const auto& [k, v] : *this)
        {
            ss << k << "\t" << v.to_string() << std::endl;
        }

        return ss.str();
    }

}
