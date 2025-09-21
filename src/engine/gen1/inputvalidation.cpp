#include <iostream>
#include <string>
#include <unordered_set>
using namespace std::string_literals;

#include "../commonvaluecollection.hpp"
#include "../commonvaluemapvalidationresult.hpp"
#include "../validationinterface.hpp"
#include "constants.hpp"

namespace StratStat
{
    void validateCommonValueMapEntry(
        const CommonValueMap& value,
        const std::string& key,
        const AllowedValues& allowedValues,
        CommonValueMapValidationResult& errResult,
        bool allowEmpty = false
    )
    {
        const auto& strValue = std::get<std::string>(value.at(key));

        if (allowEmpty && strValue.empty())
        {
            return;
        }

        if (!allowedValues.contains(strValue))
        {
            errResult.addErrorMessage(
                "invalid "s + key + " '" + strValue + "'"
            );
        }
    }

    // TODO: logging/informing about error condition?
    CommonValueMapValidationResult validatePkmnDef(const CommonValueMap& dbEntry)
    {
        CommonValueMapValidationResult errResult;

        if (dbEntry.size() < PKMN_DB_HEADERS.size())
        {
            errResult.addErrorMessage("not all expected members found"s);
        }

        validateCommonValueMapEntry(dbEntry, PKMN_TYPE1, PKMN_TYPES, errResult);
        validateCommonValueMapEntry(dbEntry, PKMN_TYPE2, PKMN_TYPES, errResult, true);
        validateCommonValueMapEntry(dbEntry, PKMN_EXPGROUP, PKMN_EXPERIENCE_GROUPS, errResult);

        if (errResult.getErrorMessages().empty())
        {
            return CommonValueCollection(PKMN_IDENTIFIER, dbEntry);
        }
        else
        {
            return errResult;
        }
    }

    CommonValueMapValidationResult validateMoveDef(const CommonValueMap& dbEntry)
    {
        CommonValueMapValidationResult errResult;

        if (dbEntry.size() < MOVE_DB_HEADERS.size())
        {
            errResult.addErrorMessage("not all expected members found"s);
        }

        validateCommonValueMapEntry(dbEntry, MOVE_TYPE_ID, PKMN_TYPES, errResult);
        validateCommonValueMapEntry(dbEntry, MOVE_DAMAGE_CLASS_ID, MOVE_DAMAGE_CLASSIDS, errResult);
        validateCommonValueMapEntry(dbEntry, MOVE_EFFECT_ID, MOVE_EFFECT_IDS, errResult);

        if (errResult.getErrorMessages().empty())
        {
            return CommonValueCollection(MOVE_IDENTIFIER, dbEntry);
        }
        else
        {
            return errResult;
        }
    }

    void getValidatedTeamDef(const jsonxx::Object& json)
    {
        std::cout << "### TODO" << std::endl;
    }
}
