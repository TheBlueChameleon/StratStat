#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <unordered_set>

#include <jsonxx.h>

#include "constants.hpp"

namespace JsonValidation
{
    class Node;
}

namespace std
{
    template <> struct hash<JsonValidation::Node>
    {
        size_t operator()(const JsonValidation::Node& node) const;
    };
}

namespace JsonValidation
{
    class Node
    {
        private:
            std::string name;
            bool mandatory;
            JsonValueType type;

            std::unordered_set<Node> children;
            std::unordered_set<std::string> mutuallyExclusiveGroups;

        public:
            Node(
                const std::string& name,
                const JsonValueType type = Object,
                const bool mandatory = true,
                const std::unordered_set<std::string>& mutuallyExclusiveGroups = {}
            );

            std::string getName() const;
            void setName(const std::string& newName);

            bool isMandatory() const;
            void setMandatory(bool newMandatory);

            JsonValueType getType() const;
            void setType(const JsonValueType& newType);

            const std::unordered_set<std::string>& getMutuallyExclusiveGroups() const;
            void addMutuallyExclusiveGroup(const std::string& groupName);
            void resetMutuallyExclusiveGruops();

            void addChild(const Node& child);
            const std::unordered_set<Node>& getChildren() const;

            bool operator==(const Node& other) const;
    };
}

#endif // NODE_HPP
