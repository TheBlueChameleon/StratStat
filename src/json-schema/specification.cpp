#include "specification.hpp"

namespace std
{
    size_t std::hash<JsonValidation::Specification>::operator()(const JsonValidation::Specification& node) const
    {
        {
            return hash<std::string>()(node.getName());
        }
    }
}

namespace JsonValidation
{
    Specification::Specification(
        const std::string& name,
        const JsonValueType type,
        const bool mandatory,
        const std::unordered_set<std::string>& mutuallyExclusiveGroups
    ):
        name(name),
        type(type),
        mandatory(mandatory),
        mutuallyExclusiveGroups(mutuallyExclusiveGroups)
    {}

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

    const MutexGroup &Specification::getMutuallyExclusiveGroups() const
    {
        return mutuallyExclusiveGroups;
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
            case JsonValueType::OBJECT_:
                children.insert(child);
                break;

            case JsonValueType::ARRAY_:
                if (children.size() > 0)
                {
                    throw std::runtime_error("An ARRAY can have only one child type");
                }
                children.insert(child);
                break;

            case JsonValueType::NUMBER_:
            case JsonValueType::STRING_:
            case JsonValueType::BOOL_:
            case JsonValueType::NULL_:
            case JsonValueType::INVALID_:
            default:
                throw std::runtime_error("Only OBJECT and ARRAY can have children");
        }
    }

    const SpecificationSet &Specification::getChildren() const
    {
        return children;
    }

    bool Specification::operator==(const Specification& other) const
    {
        return this->name == other.name;
    }
}
