#include <iostream>
#include <string>
#include <unordered_set>
using namespace std::string_literals;

#include <spdlog/spdlog.h>

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

    CommonValueMap getDefaultPlayerDef(const std::string& key)
    {
        CommonValueMap result;

        if (key == PLAYER_HUMAN)
        {
            result[PLAYER_BOOST_ATK] = true;
            result[PLAYER_BOOST_DEF] = true;
            result[PLAYER_BOOST_SPC] = true;
            result[PLAYER_BOOST_SPD] = true;
            result[PLAYER_STATUS_MOVES_DEBUFF] = false;
            result[PLAYER_USE_PP] = true;
        }
        else if (key == PLAYER_COMPUTER)
        {
            result[PLAYER_BOOST_ATK] = false;
            result[PLAYER_BOOST_DEF] = false;
            result[PLAYER_BOOST_SPC] = false;
            result[PLAYER_BOOST_SPD] = false;
            result[PLAYER_STATUS_MOVES_DEBUFF] = true;
            result[PLAYER_USE_PP] = false;
        }

        return result;
    }

    std::unordered_set<std::string> fetchBadges(const jsonxx::Array& jsonBadges)
    {
        std::unordered_set<std::string> badges;
        std::transform(
            jsonBadges.values().begin(), jsonBadges.values().end(),
            std::inserter(badges, badges.end()),
            [](const jsonxx::Value* jsonValue)
        {
            return jsonValue->get<jsonxx::String>();
        });
        return badges;
    }

    void validateAndTransferPlayerDef(
        const jsonxx::Object& json,
        const std::string& key,
        CommonValueMap& playerDef
    )
    {
        playerDef = getDefaultPlayerDef(key);

#define TRANSFER_BOOLEAN(KEY) if (json.has<jsonxx::Boolean>(KEY)) {playerDef[KEY] = json.get<jsonxx::Boolean>(KEY);}
        TRANSFER_BOOLEAN(PLAYER_BOOST_ATK);
        TRANSFER_BOOLEAN(PLAYER_BOOST_DEF);
        TRANSFER_BOOLEAN(PLAYER_BOOST_SPC);
        TRANSFER_BOOLEAN(PLAYER_BOOST_SPD);

        TRANSFER_BOOLEAN(PLAYER_STATUS_MOVES_DEBUFF);
        TRANSFER_BOOLEAN(PLAYER_USE_PP);
#undef TRANSFER_BOOLEAN

        if (json.has<jsonxx::Array>(PLAYER_BADGES))
        {
            std::unordered_set<std::string> badges = fetchBadges(json.get<jsonxx::Array>(PLAYER_BADGES));
#define APPLY_BADGE(BADGE, BOOST) if (badges.contains(BADGE)) {playerDef[BOOST] = true;}
            APPLY_BADGE(PLAYER_BADGE_BROCK, PLAYER_BOOST_ATK);
            APPLY_BADGE(PLAYER_BADGE_SURGE, PLAYER_BOOST_DEF);
            APPLY_BADGE(PLAYER_BADGE_KOGA, PLAYER_BOOST_SPD);
            APPLY_BADGE(PLAYER_BADGE_BLAINE, PLAYER_BOOST_SPC);
#undef APPLY_BADGE
        }
    }

    void validateAndTransferTeamDef(
        const jsonxx::Array& json,
        CommonValueMap& teamDef
    ) {}

    void validateAndTransferPlayerAndTeamDef(
        const jsonxx::Object& json,
        CommonValueMap& playerDef,
        CommonValueMapVector& teamDef)
    {
        if (json.has<jsonxx::Object>(PLAYER_HUMAN))
        {
            validateAndTransferPlayerDef(
                json.get<jsonxx::Object>(PLAYER_HUMAN),
                PLAYER_HUMAN, playerDef
            );
        }
        else if (json.has<jsonxx::Object>(PLAYER_COMPUTER))
        {
            validateAndTransferPlayerDef(
                json.get<jsonxx::Object>(PLAYER_COMPUTER),
                PLAYER_COMPUTER, playerDef
            );
        }
        else
        {
            throw std::runtime_error(
                "Illegal State: neither "s + PLAYER_HUMAN + " nor " + PLAYER_COMPUTER + " defined."
            );
        }
    }
}
