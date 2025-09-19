#include <iostream>
#include <string>
using namespace std::string_literals;

#include <engine/interface.hpp>

#include "../shared.hpp"
#include "../sharedtypes.hpp"
#include "constants.hpp"

extern "C" {

    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo(PKMN_IDENTIFIER, VariantContentID::Text),
            VariantContentInfo(PKMN_TYPE1,      VariantContentID::Text),
            VariantContentInfo(PKMN_TYPE2,      VariantContentID::Text),
            VariantContentInfo(PKMN_HP,         VariantContentID::Integer),
            VariantContentInfo(PKMN_ATK,        VariantContentID::Integer),
            VariantContentInfo(PKMN_DEF,        VariantContentID::Integer),
            VariantContentInfo(PKMN_SPC,        VariantContentID::Integer),
            VariantContentInfo(PKMN_SPD,        VariantContentID::Integer),
            VariantContentInfo(PKMN_EXPGROUP,   VariantContentID::Text),
        };
    }

    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo(MOVE_IDENTIFIER,     VariantContentID::Text),
            VariantContentInfo("type_id",           VariantContentID::Text),
            VariantContentInfo("power",             VariantContentID::Integer),
            VariantContentInfo("pp",                VariantContentID::Integer),
            VariantContentInfo("accuracy",          VariantContentID::Integer),
            VariantContentInfo("priority",          VariantContentID::Integer),
            VariantContentInfo("damage_class_id",   VariantContentID::Text),
            VariantContentInfo("effect_id",         VariantContentID::Text),
            VariantContentInfo("effect_chance",     VariantContentID::Integer),
        };
    }

    void addPlayerStructureElements(JsonValidation::Specification& player)
    {
        using namespace JsonValidation;

        const auto badgeNames = AllowedValues({"Brock", "Misty", "Surge", "Erika", "Sabrina", "Koga", "Blaine", "Giovanni"});
        const auto allAllowed = AllowedValues();

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

    void getTeamDefStructure(std::unordered_set<JsonValidation::Specification>& specs)
    {
        using namespace JsonValidation;

        const auto mutexHuman = MutexGroup({"playerHumam"});
        const auto mutexCompu = MutexGroup({"playerComputer"});

        const auto allAllowed = AllowedValues();

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
    getSignature();
    isReady();
    connectLogger(nullptr);
}
