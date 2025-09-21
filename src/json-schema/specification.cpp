#include "specification.hpp"
#include "jsonsetoperations.hpp"

namespace JsonValidation
{
    Specification::Specification(
        const std::string& name,
        const JsonValueType type,
        const bool mandatory,
        const AllowedValues& allowedValues,
        const MutexGroup& mutuallyExclusiveGroups
    ):
        name(name),
        type(type),
        mandatory(mandatory),
        mutuallyExclusiveGroups(mutuallyExclusiveGroups),
        allowedValues(allowedValues)
    {
        AllowedValues x = {};
    }

    std::string Specification::getName() const
    {
        return name;
    }

    void Specification::setName(const std::string& newName)
    {
        name = newName;
    }

    bool Specification::isMandatory() const
    {
        return mandatory;
    }

    void Specification::setMandatory(bool newMandatory)
    {
        mandatory = newMandatory;
    }

    JsonValueType Specification::getType() const
    {
        return type;
    }

    void Specification::setType(const JsonValueType& newType)
    {
        children.clear();
        type = newType;
    }

    const AllowedValues& Specification::getAllowedValues() const
    {
        return allowedValues;
    }

    void Specification::setAllowedValues(const AllowedValues& newAllowedValues)
    {
        for (const auto& valueSpec: newAllowedValues)
        {
            valueSpec.type_;
        }

        allowedValues = newAllowedValues;
    }

    const MutexGroup& Specification::getMutuallyExclusiveGroups() const
    {
        return mutuallyExclusiveGroups;
    }

    void Specification::setMutuallyExclusiveGroups(const MutexGroup& newMutuallyExclusiveGroups)
    {
        mutuallyExclusiveGroups = newMutuallyExclusiveGroups;
    }

    void Specification::addMutuallyExclusiveGroup(const std::string& groupName)
    {
        mutuallyExclusiveGroups.insert(groupName);
    }

    void Specification::resetMutuallyExclusiveGruops()
    {
        mutuallyExclusiveGroups.clear();
    }

    void Specification::addChild(const Specification& child)
    {
        switch (type)
        {
            case TID_Object:
                children.insert(child);
                break;

            case TID_Array:
                if (children.size() > 0)
                {
                    throw std::runtime_error("An ARRAY can have only one child type");
                }
                children.insert(child);
                break;

            case TID_Number:
            case TID_String:
            case TID_Boolean:
            case TID_Null:
            case TID_NoValidation:
            default:
                throw std::runtime_error("Only OBJECT and ARRAY can have children");
        }
    }

    const SpecificationSet& Specification::getChildren() const
    {
        return children;
    }

    bool Specification::operator==(const Specification& other) const
    {
        return this->name == other.name;
    }
}
