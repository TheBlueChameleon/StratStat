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
using ProcessedSpecs = std::list<Specification>;
using ValidationMessageList = std::vector<std::string>;

void validateRecursively(
    const std::string& key,
    const jsonxx::Value* value,
    const SpecificationSet& childSpecs,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
);

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

std::string makeTypeName(JsonValueType typeID)
{
    switch (typeID)
    {
        case TID_Object:
            return "Object";
        case TID_Array:
            return "Array";
        case TID_Number:
            return "Number";
        case TID_String:
            return "String";
        case TID_Boolean:
            return "Boolean";
        case TID_Null:
            return "Null";
        case TID_NoValidation:
            return "(No Validation)";

        default:
            throw std::runtime_error("Unknown Type ID");
    }
}

std::string makeJsonValueRepr(const jsonxx::Value& value)
{
    switch (value.type_)
    {
        case JsonValidation::TID_Number:
            return std::to_string(value.get<jsonxx::Number>());
        case JsonValidation::TID_String:
            return value.get<jsonxx::String>();
        case JsonValidation::TID_Boolean:
            return std::to_string(value.get<jsonxx::Boolean>());
        case JsonValidation::TID_Null:
            return "null";
        case JsonValidation::TID_Array:
            return value.get<jsonxx::Array>().json();
        case JsonValidation::TID_Object:
            return value.get<jsonxx::Object>().json();
        case JsonValidation::TID_NoValidation:
        default:
            throw std::runtime_error("Not Implemented: Hash for jsonxx value type #"s + std::to_string(value.type_));
    }
}

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

MutexGroup makeSetIntersection(
    const MutexGroup& left,
    const MutexGroup& right
)
{
    // std::set_intersection requires SORTED inputs...
    MutexGroup intersection;

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
    const KeySequence& keySequence,
    const ProcessedSpecs& processed,
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
    const MutexGroup intersection = makeSetIntersection(mutexGroup, allowedMutexTokens);
    if (intersection.empty())
    {
        // conflict detected
        const auto hasConflictWithThisSpec = [&spec](const auto& processedSpec)
        {
            const auto& specMutexGroup = processedSpec.getMutuallyExclusiveGroups();
            MutexGroup intersection = makeSetIntersection(specMutexGroup, spec.getMutuallyExclusiveGroups());
            return intersection.empty();
        };
        auto conflictingSpecs = processed | std::views::filter(hasConflictWithThisSpec);
        ProcessedKeys conflictingKeys;
        for (const auto& cSpec: conflictingSpecs)
        {
            conflictingKeys.push_back(cSpec.getName());
        }
        errors.push_back("in "s + makeKeySequenceRepr(keySequence) + ": "
                         "key '" + spec.getName() + "' may not be used with the key(s): " +
                         makeListRepr(conflictingKeys)
                        );
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
    if (expected == JsonValidation::TID_NoValidation)
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

void validateValues(
    const std::string& key,
    jsonxx::Value* value,
    const AllowedValues& allowedValues,
    const KeySequence& keySequence,
    ValidationMessageList& errors
)
{
    if (allowedValues.empty())
    {
        return;
    }

    if (!allowedValues.contains(*value))
    {
        std::list<std::string> allowedReprs;
        std::transform(allowedValues.begin(), allowedValues.end(),
                       std::back_inserter(allowedReprs),
                       makeJsonValueRepr
                      );

        errors.emplace_back(
            "in "s + makeKeySequenceRepr(keySequence) + ": "
            "invalid value '" + key + "' = '" + makeJsonValueRepr(*value) + "'. "
            "Allowed values are: " + makeListRepr(allowedReprs)
        );
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
            validateValues(
                key,
                value,
                spec.getAllowedValues(),
                keySequence,
                errors
            );
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

void validateObject(
    const jsonxx::Object& json,
    const SpecificationSet& specs,
    KeySequence& keySequence,
    ValidationMessageList& errors,
    ValidationMessageList& warnings
)
{
    ProcessedSpecs processedSpecs;
    MutexGroup currentMutexTokenSet = collectMutexTokenSet(specs);
    const KV_Map& kv_map = json.kv_map();
    const auto notAvailable = specs.end();
    for (const auto& [key, value] : kv_map)
    {
        const auto it = specs.find(key);
        if (it != notAvailable)
        {
            processedSpecs.push_back(*it);
            validateMutuallyExclusiveGrups(*it, currentMutexTokenSet, keySequence, processedSpecs, errors);

            const auto expectedType = it->getType();
            const auto typeMatch = validateType(key, value, expectedType, keySequence, errors);
            if (typeMatch)
            {
                validateValues(
                    key,
                    value,
                    it->getAllowedValues(),
                    keySequence,
                    errors
                );
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
    if (childSpecs.size() > 0)
    {
        if (expectedType == JsonValidation::TID_Object)
        {
            validateObject(
                value->get<jsonxx::Object>(),
                childSpecs,
                keySequence,
                errors,
                warnings
            );
        }
        else if (expectedType == JsonValidation::TID_Array)
        {
            validateArray(
                value->get<jsonxx::Array>(),
                *childSpecs.begin(),
                keySequence,
                errors,
                warnings
            );
        }
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
