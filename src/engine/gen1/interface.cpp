#include <iostream>
#include <string>
using namespace std::string_literals;

#include "../interface.hpp"
#include "../shared.hpp"
#include "../sharedtypes.hpp"
#include "constants.hpp"

using namespace StratStat;

extern "C" {
    void getTypeNames(AllowedValues& buffer)
    {
        buffer = PKMN_TYPES;
    }

    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer = PKMN_DB_HEADERS;
    }

    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer = MOVE_DB_HEADERS;
    }

    void addPlayerStructureElements(JsonValidation::Specification& player)
    {
        using namespace JsonValidation;

        const auto badgeNames = JsonValidation::AllowedValues(
        {
            PLAYER_BADGE_BROCK,
            PLAYER_BADGE_MISTY,
            PLAYER_BADGE_SURGE,
            PLAYER_BADGE_ERIKA,
            PLAYER_BADGE_SABRINA,
            PLAYER_BADGE_KOGA,
            PLAYER_BADGE_BLAINE,
            PLAYER_BADGE_GIOVANNI
        });
        const auto allAllowed = JsonValidation::AllowedValues();

        const auto mutexBadges = MutexGroup({"badges"});
        const auto mutexBoosts = MutexGroup({"boosts"});

        auto trainerBadges = Specification(PLAYER_BADGES, TID_Array, false, allAllowed, mutexBadges);
        trainerBadges.addChild(Specification("", TID_String, false, badgeNames));

        player.addChild(trainerBadges);
        player.addChild(Specification(PLAYER_BOOST_ATK, TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification(PLAYER_BOOST_DEF, TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification(PLAYER_BOOST_SPC, TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification(PLAYER_BOOST_SPD, TID_Boolean, false, allAllowed, mutexBoosts));

        player.addChild(Specification(PLAYER_STATUS_MOVES_DEBUFF, TID_Boolean, false));
        player.addChild(Specification(PLAYER_USE_PP, TID_Boolean, false));
    }

    void addPokemonStructureElements(JsonValidation::Specification& pokemon)
    {
        using namespace JsonValidation;

        auto pokemonDef = Specification("", TID_Object, false);

        pokemonDef.addChild(Specification(TEAM_SPECIES, TID_String));
        pokemonDef.addChild(Specification(TEAM_LEVEL, TID_Number));

        pokemonDef.addChild(Specification(TEAM_DV_ATK, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_DV_DEF, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_DV_SPC, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_DV_SPD, TID_Number, false));

        pokemonDef.addChild(Specification(TEAM_SX_HP,  TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_SX_ATK, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_SX_DEF, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_SX_SPC, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_SX_SPD, TID_Number, false));

        pokemonDef.addChild(Specification(TEAM_MOVE1, TID_String));
        pokemonDef.addChild(Specification(TEAM_MOVE2, TID_String, false));
        pokemonDef.addChild(Specification(TEAM_MOVE3, TID_String, false));
        pokemonDef.addChild(Specification(TEAM_MOVE4, TID_String, false));

        pokemonDef.addChild(Specification(TEAM_MOVE1_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE2_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE3_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE4_PP, TID_Number, false));

        pokemonDef.addChild(Specification(TEAM_MOVE1_MAX_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE2_MAX_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE3_MAX_PP, TID_Number, false));
        pokemonDef.addChild(Specification(TEAM_MOVE4_MAX_PP, TID_Number, false));

        pokemon.addChild(pokemonDef);
    }

    void getTeamDefStructure(JsonValidation::SpecificationSet& specs)
    {
        using namespace JsonValidation;

        const auto mutexHuman = MutexGroup({"playerHumam"});
        const auto mutexCompu = MutexGroup({"playerComputer"});

        const auto allAllowed = JsonValidation::AllowedValues();

        auto human = Specification(PLAYER_HUMAN, TID_Object, true, allAllowed, mutexHuman);
        auto compu = Specification(PLAYER_COMPUTER, TID_Object, true, allAllowed, mutexCompu);
        addPlayerStructureElements(human);
        addPlayerStructureElements(compu);

        auto pokemon = Specification(PLAYER_POKEMON, TID_Array);
        addPokemonStructureElements(pokemon);

        specs.insert(human);
        specs.insert(compu);
        specs.insert(pokemon);
    }

    void shutdown()
    {
        readyFlag = false;
    }
}

// this ensures "unused" functions from the static lib are still linked into the dyLib.
void dummyCalls()
{
    init("", "");
    getSignature();
    isReady();
    connectLogger(nullptr);
}
