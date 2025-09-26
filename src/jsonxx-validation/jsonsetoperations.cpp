#include <list>
#include <sstream>
#include <string>
using namespace std::string_literals;

#include "jsonsetoperations.hpp"
#include "specification.hpp"

namespace std
{
    size_t std::hash<JsonValidation::Specification>::operator()(const JsonValidation::Specification& spec) const
    {
        {
            return hash<std::string>()(spec.getName());
        }
    }

    size_t std::hash<jsonxx::Value>::operator()(const jsonxx::Value& value) const
    {
        switch (value.type_)
        {
            case JsonValidation::TID_Number:
                return std::hash<jsonxx::Number> {}(value.get<jsonxx::Number>());
            case JsonValidation::TID_String:
                return std::hash<jsonxx::String> {}(value.get<jsonxx::String>());
            case JsonValidation::TID_Boolean:
                return std::hash<jsonxx::Boolean> {}(value.get<bool>());
            case JsonValidation::TID_Null:
                return 0;
            case JsonValidation::TID_Array:
                return std::hash<jsonxx::String> {}(value.get<jsonxx::Array>().json());
            case JsonValidation::TID_Object:
                return std::hash<jsonxx::String> {}(value.get<jsonxx::Object>().json());
            case JsonValidation::TID_NoValidation:
            default:
                throw std::runtime_error("Not Implemented: Hash for jsonxx value type #"s + std::to_string(value.type_));
        }
    }
}

namespace jsonxx
{
    bool operator==(const Value& lhs, const Value& rhs)
    {
        if (lhs.type_ != rhs.type_)
        {
            return false;
        }

#define CMP_AS(TYPE)          (lhs.get<TYPE>()          == rhs.get<TYPE>()         )
#define CMP_VIA(TYPE, METHOD) (lhs.get<TYPE>().METHOD() == rhs.get<TYPE>().METHOD())

        switch (lhs.type_)
        {
            case JsonValidation::TID_Number:
                return CMP_AS(jsonxx::Number);
            case JsonValidation::TID_String:
                return CMP_AS(jsonxx::String);
            case JsonValidation::TID_Boolean:
                return CMP_AS(jsonxx::Boolean);
            case JsonValidation::TID_Null:
                return true;
            case JsonValidation::TID_Array:
                return CMP_VIA(jsonxx::Array, values);
            case JsonValidation::TID_Object:
                return CMP_VIA(jsonxx::Object, kv_map);
            case JsonValidation::TID_NoValidation:
            default:
                throw std::runtime_error("Not Implemented: Hash for jsonxx value type #"s + std::to_string(lhs.type_));
        }

#undef CMP_AS
#undef CMP_VIA
    }
}
