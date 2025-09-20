#include <iostream>
#include <unordered_set>

#include "../commonvaluecollection.hpp"
#include "../innerinterface.hpp"
#include "constants.hpp"

bool validateType(const VariantContentType& value, const bool allowEmptyType = false)
{
    static const std::unordered_set<std::string> allowedTypes = PKMN_TYPES;

    const auto typeStr = std::get<std::string>(value);
    if (allowEmptyType && typeStr.empty())
    {
        return true;
    }

    {
        return allowedTypes.contains(typeStr);
    }
}

// TODO: logging/informing about error condition?
std::optional<CommonValueCollection> validatePkmnDef(const CommonValueMap& dbEntry)
{
#define INVALID return std::optional<CommonValueCollection>()

    if (dbEntry.size() < PKMN_DB_HEADERS.size())
    {
        INVALID;
    }

    if (!validateType(dbEntry.at(PKMN_TYPE1)))
    {
        INVALID;
    }
    if (!validateType(dbEntry.at(PKMN_TYPE2), true))
    {
        INVALID;
    }
#undef INVALID

    return CommonValueCollection(PKMN_IDENTIFIER, dbEntry);
}
std::optional<CommonValueCollection> validateMoveDef(const CommonValueMap& dbEntry)
{
    // TODO
    return CommonValueCollection(MOVE_IDENTIFIER, dbEntry);
}

void getValidatedTeamDef(const jsonxx::Object& json)
{
    std::cout << "### TODO" << std::endl;
}
