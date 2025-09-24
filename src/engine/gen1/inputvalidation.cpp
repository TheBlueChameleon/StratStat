#include <iostream>
#include <string>
#include <unordered_set>
using namespace std::string_literals;

#include <spdlog/spdlog.h>

#include <json-schema/specification.hpp>

#include "../commonvaluecollection.hpp"
#include "../commonvaluemapvalidationresult.hpp"
#include "../engine.hpp"
#include "../interface.hpp"
#include "../validationinterface.hpp"
#include "constants.hpp"

namespace StratStat
{
    // TODO pass through file information
    void validateCommonValueMapEntry(
        const CommonValueMap& value,
        const std::string& key,
        const AllowedValues& allowedValues,
        CommonValueMapValidationResult& errResult,
        bool allowEmpty = false
    )
    {
        const auto& strValue = value.at(key).asString();

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

    // TODO pass through file information
    void validateCommonValueMapPositiveIntEntry(
        const CommonValueMap& value,
        const std::string& key
    )
    {
        const auto intValue = value.at(key).asInt();
        if (intValue < 0)
        {
            spdlog::warn(
                "invalid "s + key + " '" + std::to_string(intValue) + "'"
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

        validateCommonValueMapPositiveIntEntry(dbEntry, PKMN_HP);
        validateCommonValueMapPositiveIntEntry(dbEntry, PKMN_ATK);
        validateCommonValueMapPositiveIntEntry(dbEntry, PKMN_DEF);
        validateCommonValueMapPositiveIntEntry(dbEntry, PKMN_SPC);
        validateCommonValueMapPositiveIntEntry(dbEntry, PKMN_SPD);

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

        validateCommonValueMapPositiveIntEntry(dbEntry, MOVE_POWER);
        validateCommonValueMapPositiveIntEntry(dbEntry, MOVE_PP);
        validateCommonValueMapPositiveIntEntry(dbEntry, MOVE_ACCURACY);
        validateCommonValueMapPositiveIntEntry(dbEntry, MOVE_PRIORITY);
        validateCommonValueMapPositiveIntEntry(dbEntry, MOVE_EFFECT_CHANCE);

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

    void transferPlayerDef(
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

    template<typename T, typename C=T, typename D>
    VariantContentType fetchValueOrDefault(
        const jsonxx::Object& pkmnDef,
        const std::string& key,
        const D defaultValue = std::monostate()
    )
    {
        if (pkmnDef.has<T>(key))
        {
            return static_cast<C>(pkmnDef.get<T>(key));
        }
        else
        {
            return defaultValue;
        }
    }

    CommonValueMap getTeamMember(const jsonxx::Object& pkmnDef)
    {
        CommonValueMap result;

#define FETCH(KEY, TYPE) result[KEY] = fetchValueOrDefault<TYPE, TYPE, std::monostate>(pkmnDef, KEY)
#define FETCH_AS(KEY, TYPE, CAST) result[KEY] = fetchValueOrDefault<TYPE, CAST, std::monostate>(pkmnDef, KEY)
#define FETCH_OR(KEY, TYPE, DEFAULT) result[KEY] = fetchValueOrDefault<TYPE>(pkmnDef, KEY, DEFAULT)
        FETCH(TEAM_SPECIES, jsonxx::String);
        FETCH_AS(TEAM_LEVEL, jsonxx::Number, int);
        FETCH_AS(TEAM_HP, jsonxx::Number, int);
        FETCH_AS(TEAM_ATK, jsonxx::Number, int);
        FETCH_AS(TEAM_DEF, jsonxx::Number, int);
        FETCH_AS(TEAM_SPC, jsonxx::Number, int);
        FETCH_AS(TEAM_SPD, jsonxx::Number, int);
        FETCH_AS(TEAM_DV_ATK, jsonxx::Number, int);
        FETCH_AS(TEAM_DV_DEF, jsonxx::Number, int);
        FETCH_AS(TEAM_DV_SPC, jsonxx::Number, int);
        FETCH_AS(TEAM_DV_SPD, jsonxx::Number, int);
        FETCH_AS(TEAM_SX_HP, jsonxx::Number, int);
        FETCH_AS(TEAM_SX_ATK, jsonxx::Number, int);
        FETCH_AS(TEAM_SX_DEF, jsonxx::Number, int);
        FETCH_AS(TEAM_SX_SPC, jsonxx::Number, int);
        FETCH_AS(TEAM_SX_SPD, jsonxx::Number, int);
        FETCH(TEAM_MOVE1, jsonxx::String);
        FETCH(TEAM_MOVE2, jsonxx::String);
        FETCH(TEAM_MOVE3, jsonxx::String);
        FETCH(TEAM_MOVE4, jsonxx::String);
        FETCH_AS(TEAM_MOVE1_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE2_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE3_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE4_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE1_MAX_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE2_MAX_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE3_MAX_PP, jsonxx::Number, int);
        FETCH_AS(TEAM_MOVE4_MAX_PP, jsonxx::Number, int);
        FETCH_OR(TEAM_STATUS, jsonxx::String, STATUS_NONE);
        FETCH_AS(TEAM_SLEEP_COUNTER, jsonxx::Number, int);
        FETCH_AS(TEAM_TOXIC_COUNTER, jsonxx::Number, int);
#undef FETCH_OR
#undef FETCH_AS
#undef FETCH

        return result;
    }

    void assertPositiveInt(
        CommonValueMap& pkmn,
        const std::string& key,
        const std::string& filename,
        const bool disallowZero = false,
        const bool force = false
    )
    {
        if (!pkmn[key].hasValue())
        {
            return;
        }

        if (pkmn[key].asInt() < (0 + disallowZero))
        {
            if (force)
            {
                spdlog::error("In team definition file '"s + filename + "': "
                              "Value for " + key + " is " +
                              (disallowZero ? "negative" : "less than one") +
                              ". (Value overridden and set to 1)");
                pkmn[key] = 1;
            }
            else
            {
                spdlog::warn("In team definition file '"s + filename + "': "
                             "Value for " + key + " is " +
                             (disallowZero ? "negative" : "less than one") +
                             " and might cause invalid states.");
            }
        }
    }

    void assertSaneMove(CommonValueMap& pkmn, const std::string& key, const std::string& filename)
    {
        if (!pkmn[key].hasValue())
        {
            return;
        }

        const auto& moveDb = Engine::getInstance().getMoveDb();
        const auto& moveName = pkmn[key].asString();
        if (!moveDb.contains(moveName))
        {
            spdlog::error("In team definition file '"s + filename + "': "
                          "Unknown move: '"s + moveName + "' (will be ignored)");
            pkmn[key] = std::monostate();
        }
    }

    void assertHasAnyMove(CommonValueMap& pkmn,const std::string& filename)
    {
        if (!(
                pkmn[TEAM_MOVE1].hasValue() ||
                pkmn[TEAM_MOVE2].hasValue() ||
                pkmn[TEAM_MOVE3].hasValue() ||
                pkmn[TEAM_MOVE4].hasValue()
            ))
        {
            spdlog::error("In team definition file '"s + filename + "': "
                          "No valid move. Pokémon of species '"s + pkmn[TEAM_SPECIES].asString() + "' will be ignored.");
            pkmn[TEAM_SPECIES] = std::monostate();
        }
    }

    void assertStateWithCounter(
        CommonValueMap& pkmn,
        const std::string& key,
        const std::string& associatedState,
        const std::string& filename,
        const bool force = false
    )
    {
        if (!pkmn[key].hasValue())
        {
            return;
        }

        bool notAssociatedWithState = false;
        if (!pkmn[associatedState].hasValue())
        {
            notAssociatedWithState = true;
        }
        else if (!pkmn[TEAM_STATUS].isString())
        {
            notAssociatedWithState = true;
        }
        else if (pkmn[TEAM_STATUS].asString() != associatedState)
        {
            notAssociatedWithState = true;
        }

        if (notAssociatedWithState)
        {
            spdlog::warn("In team definition file '"s + filename + "': "
                         "'" + key + "' was set for Pokémon of species '" + pkmn[TEAM_SPECIES].asString() + "', "
                         "but it does not also have the associated " + TEAM_STATUS + " '" + associatedState + "'. "
                         "The setting will be ignored."
                        );
            pkmn[associatedState] = std::monostate();
            return;
        }
    }

    void assertSanePkmn(CommonValueMap& pkmn, const std::string& filename)
    {
        // pkmn in db or reset to monostate
        const auto& pkmnDb = Engine::getInstance().getPkmnDb();
        const auto& species = pkmn[TEAM_SPECIES].asString();
        if (!pkmnDb.contains(species))
        {
            spdlog::error("In team definition file '"s + filename + "': "
                          "Unknown species: '"s + species + "' (will be ignored)");
            pkmn[TEAM_SPECIES] = std::monostate();
        }

        // level greater or equal to 0 or warn
        assertPositiveInt(pkmn, TEAM_LEVEL, filename);

        // move in db or reset to monostate
        assertSaneMove(pkmn, TEAM_MOVE1, filename);
        assertSaneMove(pkmn, TEAM_MOVE2, filename);
        assertSaneMove(pkmn, TEAM_MOVE3, filename);
        assertSaneMove(pkmn, TEAM_MOVE4, filename);

        // has at least one defined move or invalidate pkmn
        assertHasAnyMove(pkmn, filename);

        // effort stats positive or warn
        assertPositiveInt(pkmn, TEAM_DV_ATK, filename);
        assertPositiveInt(pkmn, TEAM_DV_DEF, filename);
        assertPositiveInt(pkmn, TEAM_DV_SPC, filename);
        assertPositiveInt(pkmn, TEAM_DV_SPD, filename);
        assertPositiveInt(pkmn, TEAM_SX_HP, filename);
        assertPositiveInt(pkmn, TEAM_SX_ATK, filename);
        assertPositiveInt(pkmn, TEAM_SX_DEF, filename);
        assertPositiveInt(pkmn, TEAM_SX_SPC, filename);
        assertPositiveInt(pkmn, TEAM_SX_SPD, filename);

        // absolute stats positive (>0) or reset to 1
        assertPositiveInt(pkmn, TEAM_HP, filename, true, true);
        assertPositiveInt(pkmn, TEAM_ATK, filename, true, true);
        assertPositiveInt(pkmn, TEAM_DEF, filename, true, true);
        assertPositiveInt(pkmn, TEAM_SPC, filename, true, true);
        assertPositiveInt(pkmn, TEAM_SPD, filename, true, true);

        // sleep counter only with status = sleep
        assertStateWithCounter(pkmn, TEAM_SLEEP_COUNTER, STATUS_SLEEP, filename, true);
        // sleep counter positive or reset to 1
        assertPositiveInt(pkmn, TEAM_SLEEP_COUNTER, filename, false, true);

        // toxic counter only with status = poison
        assertStateWithCounter(pkmn, TEAM_TOXIC_COUNTER, STATUS_POISON, filename);
        // toxic counter positive or warn
        assertPositiveInt(pkmn, TEAM_TOXIC_COUNTER, filename, false, false);
    }

    void transferTeamDef(
        const jsonxx::Array& json,
        const std::string& filename,
        CommonValueMapVector& teamDef
    )
    {
        const auto N = json.values().size();
        if (N > 6)
        {
            spdlog::warn("In team definition file '"s + filename + "': "
                         "Non-standard team with "s + std::to_string(N) + " members!");
        }
        teamDef.reserve(N);

        for (const jsonxx::Value* ptr : json.values())
        {
            auto pkmn = getTeamMember(ptr->get<jsonxx::Object>());
            assertSanePkmn(pkmn, filename);
            if (pkmn.at(TEAM_SPECIES).hasValue())
            {
                teamDef.push_back(pkmn);
            }
        }

        if (teamDef.size() == 0)
        {
            spdlog::critical("In team definition file '"s + filename + "': "
                             "Team with no (valid) members!");
            std::exit(-1);
        }
    }

    void transferPlayerAndTeamDef(
        const jsonxx::Object& json,
        const std::string& filename,
        CommonValueMap& playerDef,
        CommonValueMapVector& teamDef)
    {
        if (json.has<jsonxx::Object>(PLAYER_HUMAN))
        {
            transferPlayerDef(
                json.get<jsonxx::Object>(PLAYER_HUMAN),
                PLAYER_HUMAN, playerDef
            );
        }
        else if (json.has<jsonxx::Object>(PLAYER_COMPUTER))
        {
            transferPlayerDef(
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

        transferTeamDef(
            json.get<jsonxx::Array>(PLAYER_POKEMON),
            filename,
            teamDef
        );
    }
}
