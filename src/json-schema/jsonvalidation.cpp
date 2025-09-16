#include <algorithm>
#include <forward_list>
#include <list>
#include <ranges>
#include <sstream>
#include <string>
using namespace std::string_literals;

#include "jsonvalidation.hpp"

using namespace JsonValidation;

std::string makeKeySequenceRepr(const std::list<std::string>& keySequence)
{
    if (keySequence.empty())
    {
        return "(root)";
    }

    std::stringstream buffer;

    for (const auto& key : keySequence)
    {
        buffer << "/" << key;
    }

    return buffer.str();
}

std::string makeKeyListRepr(const std::forward_list<std::string>& processed)
{
    std::stringstream buffer;

    auto it = processed.cbegin();
    buffer << "{'" << *it << "'";

    ++it;
    for (; it != processed.cend(); it = ++it)
    {
        buffer << ", '" << *it << "'";
    }
    buffer << "}";

    return buffer.str();
}

std::string makeMutexErrorMsg(
    const std::string& name,
    const std::list<std::string>& keySequence,
    const std::forward_list<std::string>& processed
)
{
    return "in "s + makeKeySequenceRepr(keySequence) + ": "
           "key '" + name + "' may not be used with one or several of the keys " +
           makeKeyListRepr(processed);
}

std::string makeTypeName(JsonValidation::JsonValueType typeID)
{
    switch (typeID)
    {
        case Object:
            return "Object";
        case Array:
            return "Array";
        case Number:
            return "Number";
        case String:
            return "String";
        case Bool:
            return "Boolean";
        case Null:
            return "Null";
        case NoValidation:
            return "(No Validation)";

        default:
            throw std::runtime_error("Unknown Type ID");
    }
}

void validateMandatoryNodes(
    const std::map<std::string, jsonxx::Value*>& kv_map,
    const std::unordered_set<Node>& node,
    const std::list<std::string>& keySequence,
    std::vector<std::string>& errors
)
{
    auto isMandatory = [](const JsonValidation::Node& node)
    {
        return node.isMandatory();
    };

    auto mandatorySpecs = node | std::views::filter(isMandatory);

    for (const auto& ms : mandatorySpecs)
    {
        const auto& key = ms.getName();
        if (!kv_map.contains(key))
        {
            errors.emplace_back("in "s + makeKeySequenceRepr(keySequence) + ": missing mandatory key '" + key + "'");
        }
    }
}

std::unordered_set<std::string> makeSetIntersection(
    const std::unordered_set<std::string>& left,
    const std::unordered_set<std::string>& right
)
{
    // std::set_intersection requires SORTED inputs...
    std::unordered_set<std::string> intersection;

    for (const auto& l : left)
    {
        if (right.contains(l))
        {
            intersection.insert(l);
        }
    }

    return intersection;
}

void validateMutuallyExclusiveGrups(
    const JsonValidation::Node& node,
    std::unordered_set<std::string>& allowedMutexTokens,
    const std::list<std::string>& keySequence,
    const std::forward_list<std::string>& processed,
    std::vector<std::string>& errors
)
{
    const auto& spec = node.getMutuallyExclusiveGroups();
    if (spec.empty())
    {
        // current item does not belong to any mutually exclusive group -- pass
        return;
    }

    if (allowedMutexTokens.empty())
    {
        // no mutex group was identified -- make this the current group
        allowedMutexTokens = spec;
        return;
    }

    if (spec.size() == 1)
    {
        const auto& group = *spec.begin();
        if (allowedMutexTokens.contains(group))
        {
            // only one spec allowed from now on
            allowedMutexTokens = spec;
        }
        else
        {
            errors.push_back(makeMutexErrorMsg(node.getName(), keySequence, processed));
        }
    }
    else
    {
        // current node can belong to several mutex groups: is any of them the current group?
        std::unordered_set<std::string> intersection = makeSetIntersection(spec, allowedMutexTokens);
        if (intersection.empty())
        {
            // conflict detected
            errors.push_back(makeMutexErrorMsg(node.getName(), keySequence, processed));
        }
        else
        {
            // narrow down set of allowed groups
            allowedMutexTokens = intersection;
        }
    }
}

bool validateType(
    const std::string& key,
    const jsonxx::Value* value,
    const JsonValidation::JsonValueType expected,
    const std::list<std::string>& keySequence,
    std::vector<std::string>& errors
)
{
    if (expected == JsonValidation::NoValidation)
    {
        return true;
    }

    const auto actual = value->type_;
    if (expected != actual)
    {
        errors.push_back(
            "in "s + makeKeySequenceRepr(keySequence) + ": "
            "type of key '" + key + "' is of type " + makeTypeName(actual) +
            " but expected type is " + makeTypeName(expected)
        );
        return false;
    }
    return true;
}

void validateRecursively(
    const std::string& key,
    const jsonxx::Value* value,
    const std::unordered_set<Node>& childSpecs,
    std::list<std::string>& keySequence,
    std::vector<std::string>& errors,
    std::vector<std::string>& warnings
);

void validateArray(
    const jsonxx::Array& json,
    const Node& spec,
    std::list<std::string>& keySequence,
    std::vector<std::string>& errors,
    std::vector<std::string>& warnings
)
{
    const auto expectedType = spec.getType();

    int i = 0;
    for (const auto value : json.values())
    {
        const auto key = "item #"s + std::to_string(i);
        const auto matchingType = validateType(
                                      key,
                                      value,
                                      expectedType,
                                      keySequence,
                                      errors
                                  );


        if (matchingType)
        {
            validateRecursively(
                key,
                value,
                spec.getChildren(),
                keySequence,
                errors,
                warnings
            );
        }
        ++i;
    }
}

void validateInternal(
    const jsonxx::Object& json,
    const std::unordered_set<Node>& specs,
    std::list<std::string>& keySequence,
    std::vector<std::string>& errors,
    std::vector<std::string>& warnings
)
{
    const auto& kv_map = json.kv_map();
    validateMandatoryNodes(kv_map, specs, keySequence, errors);

    std::forward_list<std::string> processed;
    std::unordered_set<std::string> currentMutexTokenSet;
    for (const auto& [key, value] : kv_map)
    {
        if (specs.contains(key))
        {
            const auto it = specs.find(key);
            processed.push_front(key);
            validateMutuallyExclusiveGrups(*it, currentMutexTokenSet, keySequence, processed, errors);

            const auto expectedType = it->getType();
            const auto typeMatch = validateType(key, value, expectedType, keySequence, errors);
            if (typeMatch)
            {
                validateRecursively(
                    key,
                    value,
                    it->getChildren(),
                    keySequence,
                    errors,
                    warnings
                );
            }
        }
        else
        {
            warnings.emplace_back(
                "in "s + makeKeySequenceRepr(keySequence) + ": found unknown key '" + key + "'"
            );
        }
    }
}

void validateRecursively(
    const std::string& key,
    const jsonxx::Value* value,
    const std::unordered_set<Node>& childSpecs,
    std::list<std::string>& keySequence,
    std::vector<std::string>& errors,
    std::vector<std::string>& warnings
)
{
    keySequence.push_back(key);
    const auto expectedType = value->type_;
    if (expectedType == JsonValidation::Object)
    {
        validateInternal(
            value->get<jsonxx::Object>(),
            childSpecs,
            keySequence,
            errors,
            warnings
        );
    }
    else if (expectedType == JsonValidation::Array)
    {
        validateArray(
            value->get<jsonxx::Array>(),
            *childSpecs.begin(),
            keySequence,
            errors,
            warnings
        );
    }
    keySequence.pop_back();
}

namespace JsonValidation
{
    JsonValidationResult validate(const jsonxx::Object& json, const std::unordered_set<Node>& specs)
    {
        std::list<std::string> keySequence;
        std::vector<std::string> errors;
        std::vector<std::string> warnings;

        validateInternal(json, specs, keySequence, errors, warnings);

        return JsonValidationResult(errors, warnings);
    }
}
