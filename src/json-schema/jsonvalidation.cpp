#include <algorithm>
#include <list>
#include <ranges>
#include <sstream>
#include <string>
using namespace std::string_literals;

#include "jsonvalidation.hpp"

using namespace JsonValidation;

using KV_Map = std::map<std::string, jsonxx::Value*>;
using KeySequence = std::list<std::string>;
using ProcessedKeys = std::list<std::string>;
using ValidationMessageList = std::vector<std::string>;

std::string makeKeySequenceRepr(const KeySequence& keySequence)
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

std::string makeListRepr(auto& items)
{
    std::stringstream buffer;

    auto it = items.begin();
    buffer << "{'" << *it << "'";

    ++it;
    for (; it != items.end(); it = ++it)
    {
        buffer << ", '" << *it << "'";
    }
    buffer << "}";

    return buffer.str();
}

// TODO: list only the forbidden keys given the current token set
std::string makeMutexErrorMsg(
    const std::string& name,
    const KeySequence& keySequence,
    const ProcessedKeys& processed
)
{
    return "in "s + makeKeySequenceRepr(keySequence) + ": "
           "key '" + name + "' may not be used with one or several of the keys " +
           makeListRepr(processed);
}

std::string makeTypeName(JsonValueType typeID)
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

void validateMandatorySpecs(
    const KV_Map& kv_map,
    const SpecificationSet& specs,
    const MutexGroup& currentMutexTokenSet,
    const KeySequence& keySequence,
    ValidationMessageList& errors
)
{
    auto isMandatory = [](const JsonValidation::Specification& node)
    {
        return node.isMandatory();
    };
    auto hasNoMutexToken = [](const JsonValidation::Specification& node)
    {
        return node.getMutuallyExclusiveGroups().empty();
    };
    auto isPresent = [&kv_map](const JsonValidation::Specification& node)
    {
        return kv_map.contains(node.getName());
    };
    auto checkAndAddError = [&errors, &kv_map, &keySequence](const JsonValidation::Specification& node)
    {
        const auto& key = node.getName();
        if (!kv_map.contains(key))
        {
            errors.emplace_back("in "s + makeKeySequenceRepr(keySequence) + ": missing mandatory key '" + key + "'");
        }
    };

    auto alwaysMandatorySpecs = specs
                                | std::views::filter(isMandatory)
                                | std::views::filter(hasNoMutexToken);
    std::ranges::for_each(alwaysMandatorySpecs, checkAndAddError);

    for (const auto& token : currentMutexTokenSet)
    {
        auto hasThisMutexToken = [&token](const Specification& node)
        {
            return node.getMutuallyExclusiveGroups().contains(token);
        };

        auto mandatorySpecsWithToken = specs
                                       | std::views::filter(isMandatory)
                                       | std::views::filter(hasThisMutexToken);

        if (mandatorySpecsWithToken.empty())
        {
            // std::ranges::none_of on empty range gives UB
            continue;
        }

        if (std::ranges::none_of(mandatorySpecsWithToken, isPresent))
        {
            auto mandatoryKeyNames = mandatorySpecsWithToken | std::views::transform([](const auto& spec)
            {
                return spec.getName();
            });
            errors.emplace_back("in "s + makeKeySequenceRepr(keySequence) + ": missing one of the mandatory keys " + makeListRepr(mandatoryKeyNames));
        }
    }
}

MutexGroup makeSetIntersection(
    const MutexGroup& left,
    const MutexGroup& right
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
    const Specification& spec,
    MutexGroup& allowedMutexTokens,
    const std::list<std::string>& keySequence,
    const ProcessedKeys& processed,
    ValidationMessageList& errors
)
{
    const auto& mutexGroup = spec.getMutuallyExclusiveGroups();
    if (mutexGroup.empty())
    {
        // current item does not belong to any mutually exclusive group -- pass
        return;
    }

    // current node can belong to several mutex groups: is any of them the current group?
    MutexGroup intersection = makeSetIntersection(mutexGroup, allowedMutexTokens);
    if (intersection.empty())
    {
        // conflict detected
        // auto hasConflict = [](const auto&)
        // {
        //     return;
        // };
        // ProcessedKeys conflicting = processed | std::views::filter();
        errors.push_back(makeMutexErrorMsg(spec.getName(), keySequence, processed));
    }
    else
    {
        // narrow down set of allowed groups
        allowedMutexTokens = intersection;
    }
}

bool validateType(
    const std::string& key,
    const jsonxx::Value* value,
    const JsonValueType expected,
    const KeySequence& keySequence,
    ValidationMessageList& errors
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
    const SpecificationSet& childSpecs,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
);

void validateArray(
    const jsonxx::Array& json,
    const Specification& spec,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
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

MutexGroup collectMutexTokenSet(const SpecificationSet& specs)
{
    MutexGroup result;

    for (const auto& spec: specs)
    {
        const auto& tokens = spec.getMutuallyExclusiveGroups();
        for (const auto& token : tokens)
        {
            result.insert(token);
        }
    }

    return result;
}

void validateObject(
    const jsonxx::Object& json,
    const SpecificationSet& specs,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
)
{
    ProcessedKeys processed;
    MutexGroup currentMutexTokenSet = collectMutexTokenSet(specs);
    const KV_Map& kv_map = json.kv_map();
    const auto notAvailable = specs.end();
    for (const auto& [key, value] : kv_map)
    {
        const auto it = specs.find(key);
        if (it != notAvailable)
        {
            processed.push_back(key);
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

    validateMandatorySpecs(kv_map, specs, currentMutexTokenSet, keySequence, errors);
}

void validateRecursively(
    const std::string& key,
    const jsonxx::Value* value,
    const SpecificationSet& childSpecs,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
)
{
    keySequence.push_back(key);
    const auto expectedType = value->type_;
    if (expectedType == JsonValidation::Object)
    {
        validateObject(
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
    JsonValidationResult validate(const jsonxx::Object& json, const SpecificationSet& specs)
    {
        KeySequence keySequence;
        ValidationMessageList errors;
        ValidationMessageList warnings;

        validateObject(json, specs, keySequence, errors, warnings);

        return JsonValidationResult(errors, warnings);
    }
}
