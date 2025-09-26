#ifndef JSONSETOPERATIONS_H
#define JSONSETOPERATIONS_H

#include <utility>

#include <jsonxx.h>

namespace JsonValidation
{
    class Specification;
}

namespace std
{
    template <> struct hash<JsonValidation::Specification>
    {
        size_t operator()(const JsonValidation::Specification& specification) const;
    };

    template <> struct hash<jsonxx::Value>
    {
        size_t operator()(const jsonxx::Value& specification) const;
    };
}

namespace jsonxx
{
    bool operator==(const jsonxx::Value& lhs, const jsonxx::Value& rhs);
}

#endif // JSONSETOPERATIONS_H
