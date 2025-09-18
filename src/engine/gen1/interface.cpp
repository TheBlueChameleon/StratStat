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

    void addPlayerStructureElements(JsonValidation::Node& player)
    {
        using namespace JsonValidation;

        const auto mutexBadges = MutexGroup({"badges"});
        const auto mutexBoosts = MutexGroup({"boosts"});

        auto trainerBadges = Node("badges", Array, false, mutexBadges);
        trainerBadges.addChild(Node("", String, false));

        player.addChild(trainerBadges);
        player.addChild(Node("boostAtk", Bool, false, mutexBoosts));
        player.addChild(Node("boostDef", Bool, false, mutexBoosts));
        player.addChild(Node("boostSpd", Bool, false, mutexBoosts));
        player.addChild(Node("boostSpc", Bool, false, mutexBoosts));

        player.addChild(Node("statMoveDebuf", Bool, false));
        player.addChild(Node("usesPP", Bool, false));
    }

    void addPokemonStructureElements(JsonValidation::Node& pokemon)
    {
        using namespace JsonValidation;

        auto pokemonDef = Node("", Object, false);

        pokemonDef.addChild(Node("species", String));
        pokemonDef.addChild(Node("level", Number));

        pokemonDef.addChild(Node("DV_ATK", Number, false));
        pokemonDef.addChild(Node("DV_DEF", Number, false));
        pokemonDef.addChild(Node("DV_SPC", Number, false));
        pokemonDef.addChild(Node("DV_SPD", Number, false));

        pokemonDef.addChild(Node("StatExperience_HP",  Number, false));
        pokemonDef.addChild(Node("StatExperience_ATK", Number, false));
        pokemonDef.addChild(Node("StatExperience_DEF", Number, false));
        pokemonDef.addChild(Node("StatExperience_SPC", Number, false));
        pokemonDef.addChild(Node("StatExperience_SPD", Number, false));

        pokemonDef.addChild(Node("move1", String));
        pokemonDef.addChild(Node("move2", String, false));
        pokemonDef.addChild(Node("move3", String, false));
        pokemonDef.addChild(Node("move4", String, false));

        pokemonDef.addChild(Node("move1PP", Number, false));
        pokemonDef.addChild(Node("move2PP", Number, false));
        pokemonDef.addChild(Node("move3PP", Number, false));
        pokemonDef.addChild(Node("move4PP", Number, false));

        pokemonDef.addChild(Node("move1MaxPP", Number, false));
        pokemonDef.addChild(Node("move2MaxPP", Number, false));
        pokemonDef.addChild(Node("move3MaxPP", Number, false));
        pokemonDef.addChild(Node("move4MaxPP", Number, false));

        pokemon.addChild(pokemonDef);
    }

    void getTeamDefStructure(std::unordered_set<JsonValidation::Node>& specs)
    {
        using namespace JsonValidation;

        const auto mutexPlayerType = MutexGroup({"playerType"});

        auto trainer = Node("human", Object, true, mutexPlayerType);
        auto computer = Node("computer", Object, true, mutexPlayerType);
        addPlayerStructureElements(trainer);
        addPlayerStructureElements(computer);

        auto pokemon = Node("pokemon", Array);
        addPokemonStructureElements(pokemon);

        specs.insert(trainer);
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
