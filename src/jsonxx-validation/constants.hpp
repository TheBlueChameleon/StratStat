#ifndef JSON_SCHEMA_CONSTANTS_HPP
#define JSON_SCHEMA_CONSTANTS_HPP

#include <jsonxx.h>

namespace JsonValidation
{
    using JsonValueType = decltype(jsonxx::Value::type_);

    constexpr JsonValueType TID_Object = JsonValueType::OBJECT_;
    constexpr JsonValueType TID_Array = JsonValueType::ARRAY_;
    constexpr JsonValueType TID_Number = JsonValueType::NUMBER_;
    constexpr JsonValueType TID_String = JsonValueType::STRING_;
    constexpr JsonValueType TID_Boolean = JsonValueType::BOOL_;
    constexpr JsonValueType TID_Null = JsonValueType::NULL_;
    constexpr JsonValueType TID_NoValidation = JsonValueType::INVALID_;
}

#endif // JSON_SCHEMA_CONSTANTS_HPP
