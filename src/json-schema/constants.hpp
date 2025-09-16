#ifndef JSON_SCHEMA_CONSTANTS_HPP
#define JSON_SCHEMA_CONSTANTS_HPP

#include <jsonxx.h>

namespace JsonValidation
{
    using JsonValueType = decltype(jsonxx::Value::type_);

    constexpr JsonValueType Object = JsonValueType::OBJECT_;
    constexpr JsonValueType Array = JsonValueType::ARRAY_;
    constexpr JsonValueType Number = JsonValueType::NUMBER_;
    constexpr JsonValueType String = JsonValueType::STRING_;
    constexpr JsonValueType Bool = JsonValueType::BOOL_;
    constexpr JsonValueType Null = JsonValueType::NULL_;
    constexpr JsonValueType NoValidation = JsonValueType::INVALID_;
}

#endif // JSON_SCHEMA_CONSTANTS_HPP
