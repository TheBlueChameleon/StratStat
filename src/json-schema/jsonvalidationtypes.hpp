#ifndef JSONVALIDATIONTYPES_H
#define JSONVALIDATIONTYPES_H

#include <unordered_set>
#include <string>

#include <jsonxx.h>

namespace JsonValidation
{
    class Specification;

    class SpecificationSet : public std::unordered_set<Specification>
    {
        public:
            using std::unordered_set<Specification>::unordered_set;
    };

    class MutexGroup : public std::unordered_set<std::string>
    {
        public:
            using std::unordered_set<std::string>::unordered_set;
    };

    class AllowedValues : public std::unordered_set<jsonxx::Value>
    {
        public:
            using std::unordered_set<jsonxx::Value>::unordered_set;
    };
}

#endif // JSONVALIDATIONTYPES_H
