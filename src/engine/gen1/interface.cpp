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

        const auto mutexBadges = MutexGroup({"badges"});
        const auto mutexBoosts = MutexGroup({"boosts"});

        auto trainerBadges = Specification("badges", Array, false, mutexBadges);
        trainerBadges.addChild(Specification("", String, false));

        player.addChild(trainerBadges);
        player.addChild(Specification("boostAtk", Bool, false, mutexBoosts));
        player.addChild(Specification("boostDef", Bool, false, mutexBoosts));
        player.addChild(Specification("boostSpd", Bool, false, mutexBoosts));
        player.addChild(Specification("boostSpc", Bool, false, mutexBoosts));

        player.addChild(Specification("statMoveDebuf", Bool, false));
        player.addChild(Specification("usesPP", Bool, false));
    }

    void addPokemonStructureElements(JsonValidation::Specification& pokemon)
    {
        using namespace JsonValidation;

        auto pokemonDef = Specification("", Object, false);

        pokemonDef.addChild(Specification("species", String));
        pokemonDef.addChild(Specification("level", Number));

        pokemonDef.addChild(Specification("DV_ATK", Number, false));
        pokemonDef.addChild(Specification("DV_DEF", Number, false));
        pokemonDef.addChild(Specification("DV_SPC", Number, false));
        pokemonDef.addChild(Specification("DV_SPD", Number, false));

        pokemonDef.addChild(Specification("StatExperience_HP",  Number, false));
        pokemonDef.addChild(Specification("StatExperience_ATK", Number, false));
        pokemonDef.addChild(Specification("StatExperience_DEF", Number, false));
        pokemonDef.addChild(Specification("StatExperience_SPC", Number, false));
        pokemonDef.addChild(Specification("StatExperience_SPD", Number, false));

        pokemonDef.addChild(Specification("move1", String));
        pokemonDef.addChild(Specification("move2", String, false));
        pokemonDef.addChild(Specification("move3", String, false));
        pokemonDef.addChild(Specification("move4", String, false));

        pokemonDef.addChild(Specification("move1PP", Number, false));
        pokemonDef.addChild(Specification("move2PP", Number, false));
        pokemonDef.addChild(Specification("move3PP", Number, false));
        pokemonDef.addChild(Specification("move4PP", Number, false));

        pokemonDef.addChild(Specification("move1MaxPP", Number, false));
        pokemonDef.addChild(Specification("move2MaxPP", Number, false));
        pokemonDef.addChild(Specification("move3MaxPP", Number, false));
        pokemonDef.addChild(Specification("move4MaxPP", Number, false));

        pokemon.addChild(pokemonDef);
    }

    void getTeamDefStructure(std::unordered_set<JsonValidation::Specification>& specs)
    {
        using namespace JsonValidation;

        const auto mutexHuman = MutexGroup({"playerHumam"});
        const auto mutexCompu = MutexGroup({"playerComputer"});

        auto human = Specification("human", Object, true, mutexHuman);
        auto compu = Specification("computer", Object, true, mutexCompu);
        addPlayerStructureElements(human);
        addPlayerStructureElements(compu);

        auto pokemon = Specification("pokemon", Array);
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
