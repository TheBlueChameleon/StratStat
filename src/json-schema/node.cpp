#include "node.hpp"

namespace std
{
    size_t std::hash<JsonValidation::Node>::operator()(const JsonValidation::Node& node) const
    {
        {
            return hash<std::string>()(node.getName());
        }
    }
}

namespace JsonValidation
{
    Node::Node(
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

    std::string Node::getName() const
    {
        return name;
    }

    void Node::setName(const std::string& newName)
    {
        name = newName;
    }

    bool Node::isMandatory() const
    {
        return mandatory;
    }

    void Node::setMandatory(bool newMandatory)
    {
        mandatory = newMandatory;
    }

    JsonValueType Node::getType() const
    {
        return type;
    }

    void Node::setType(const JsonValueType& newType)
    {
        children.clear();
        type = newType;
    }

    const std::unordered_set<std::string>& Node::getMutuallyExclusiveGroups() const
    {
        return mutuallyExclusiveGroups;
    }

    void Node::addMutuallyExclusiveGroup(const std::string& groupName)
    {
        mutuallyExclusiveGroups.insert(groupName);
    }

    void Node::resetMutuallyExclusiveGruops()
    {
        mutuallyExclusiveGroups.clear();
    }

    void Node::addChild(const Node& child)
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

    const std::unordered_set<Node>& Node::getChildren() const
    {
        return children;
    }

    bool Node::operator==(const Node& other) const
    {
        return this->name == other.name;
    }
}
