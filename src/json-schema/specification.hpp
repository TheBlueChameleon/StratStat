#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <string>
#include <unordered_set>

#include <jsonxx.h>

#include "constants.hpp"

namespace JsonValidation
{
    class Specification;
    using SpecificationSet = std::unordered_set<Specification>;
    using MutexGroup = std::unordered_set<std::string>;
}

namespace std
{
    template <> struct hash<JsonValidation::Specification>
    {
        size_t operator()(const JsonValidation::Specification& specification) const;
    };
}

namespace JsonValidation
{
    class Specification
    {
        private:
            std::string name;
            bool mandatory;
            JsonValueType type;

            SpecificationSet children;
            MutexGroup mutuallyExclusiveGroups;

        public:
            Specification(
                const std::string& name,
                const JsonValueType type = Object,
                const bool mandatory = true,
                const MutexGroup& mutuallyExclusiveGroups = {}
            );

            std::string getName() const;
            void setName(const std::string& newName);

            bool isMandatory() const;
            void setMandatory(bool newMandatory);

            JsonValueType getType() const;
            void setType(const JsonValueType& newType);

            const MutexGroup& getMutuallyExclusiveGroups() const;
            void addMutuallyExclusiveGroup(const std::string& groupName);
            void resetMutuallyExclusiveGruops();

            void addChild(const Specification& child);
            const SpecificationSet& getChildren() const;

            bool operator==(const Specification& other) const;
    };
}

#endif // SPECIFICATION_H
