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

        const auto badgeNames = JsonValidation::AllowedValues({"Brock", "Misty", "Surge", "Erika", "Sabrina", "Koga", "Blaine", "Giovanni"});
        const auto allAllowed = JsonValidation::AllowedValues();

        const auto mutexBadges = MutexGroup({"badges"});
        const auto mutexBoosts = MutexGroup({"boosts"});

        auto trainerBadges = Specification("badges", TID_Array, false, allAllowed, mutexBadges);
        trainerBadges.addChild(Specification("", TID_String, false, badgeNames));

        player.addChild(trainerBadges);
        player.addChild(Specification("boostAtk", TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification("boostDef", TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification("boostSpd", TID_Boolean, false, allAllowed, mutexBoosts));
        player.addChild(Specification("boostSpc", TID_Boolean, false, allAllowed, mutexBoosts));

        player.addChild(Specification("statMoveDebuf", TID_Boolean, false));
        player.addChild(Specification("usesPP", TID_Boolean, false));
    }

    void addPokemonStructureElements(JsonValidation::Specification& pokemon)
    {
        using namespace JsonValidation;

        auto pokemonDef = Specification("", TID_Object, false);

        pokemonDef.addChild(Specification("species", TID_String));
        pokemonDef.addChild(Specification("level", TID_Number));

        pokemonDef.addChild(Specification("DV_ATK", TID_Number, false));
        pokemonDef.addChild(Specification("DV_DEF", TID_Number, false));
        pokemonDef.addChild(Specification("DV_SPC", TID_Number, false));
        pokemonDef.addChild(Specification("DV_SPD", TID_Number, false));

        pokemonDef.addChild(Specification("StatExperience_HP",  TID_Number, false));
        pokemonDef.addChild(Specification("StatExperience_ATK", TID_Number, false));
        pokemonDef.addChild(Specification("StatExperience_DEF", TID_Number, false));
        pokemonDef.addChild(Specification("StatExperience_SPC", TID_Number, false));
        pokemonDef.addChild(Specification("StatExperience_SPD", TID_Number, false));

        pokemonDef.addChild(Specification("move1", TID_String));
        pokemonDef.addChild(Specification("move2", TID_String, false));
        pokemonDef.addChild(Specification("move3", TID_String, false));
        pokemonDef.addChild(Specification("move4", TID_String, false));

        pokemonDef.addChild(Specification("move1PP", TID_Number, false));
        pokemonDef.addChild(Specification("move2PP", TID_Number, false));
        pokemonDef.addChild(Specification("move3PP", TID_Number, false));
        pokemonDef.addChild(Specification("move4PP", TID_Number, false));

        pokemonDef.addChild(Specification("move1MaxPP", TID_Number, false));
        pokemonDef.addChild(Specification("move2MaxPP", TID_Number, false));
        pokemonDef.addChild(Specification("move3MaxPP", TID_Number, false));
        pokemonDef.addChild(Specification("move4MaxPP", TID_Number, false));

        pokemon.addChild(pokemonDef);
    }

    void getTeamDefStructure(JsonValidation::SpecificationSet& specs)
    {
        using namespace JsonValidation;

        const auto mutexHuman = MutexGroup({"playerHumam"});
        const auto mutexCompu = MutexGroup({"playerComputer"});

        const auto allAllowed = JsonValidation::AllowedValues();

        auto human = Specification("human", TID_Object, true, allAllowed, mutexHuman);
        auto compu = Specification("computer", TID_Object, true, allAllowed, mutexCompu);
        addPlayerStructureElements(human);
        addPlayerStructureElements(compu);

        auto pokemon = Specification("pokemon", TID_Array);
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
