#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#include <string>
#include <unordered_set>

#include <jsonxx.h>

#include "constants.hpp"
#include "jsonsetoperations.hpp"
#include "jsonvalidationtypes.hpp"

namespace JsonValidation
{
    class Specification
    {
        private:
            std::string name;
            bool mandatory;
            JsonValueType type;
            AllowedValues allowedValues;

            SpecificationSet children;
            MutexGroup mutuallyExclusiveGroups;

        public:
            Specification(
                const std::string& name,
                const JsonValueType typeID = TID_Object,
                const bool mandatory = true,
                const AllowedValues& allowedValues = {},
                const MutexGroup& mutuallyExclusiveGroups = {}
            );

            std::string getName() const;
            void setName(const std::string& newName);

            bool isMandatory() const;
            void setMandatory(bool newMandatory);

            JsonValueType getType() const;
            void setType(const JsonValueType& newType);

            const AllowedValues& getAllowedValues() const;
            void setAllowedValues(const AllowedValues& newAllowedValues);

            const MutexGroup& getMutuallyExclusiveGroups() const;
            void setMutuallyExclusiveGroups(const MutexGroup& newMutuallyExclusiveGroups);
            void addMutuallyExclusiveGroup(const std::string& groupName);
            void resetMutuallyExclusiveGruops();

            void addChild(const Specification& child);
            const SpecificationSet& getChildren() const;

            bool operator==(const Specification& other) const;
    };
}

#endif // SPECIFICATION_H
