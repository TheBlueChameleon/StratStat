#include <iostream>
#include <string>
using namespace std::string_literals;

#include <engine/interface.hpp>

#include "constants.hpp"
#include <engine/types.hpp>

static bool readyFlag = false;

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

    void getTeamDefStructure(std::unordered_set<JsonValidation::Node>& specs)
    {
        using namespace JsonValidation;

        const auto mutexBadges = MutexGroup({"badges"});
        const auto mutexBoosts = MutexGroup({"boosts"});

        auto trainerBadges = Node("badges", Array, false, mutexBadges);
        trainerBadges.addChild(Node("", String, false));

        auto trainer = Node("trainer", Object);
        trainer.addChild(trainerBadges);
        trainer.addChild(Node("boostAtk", Bool, false, mutexBoosts));
        trainer.addChild(Node("boostDef", Bool, false, mutexBoosts));
        trainer.addChild(Node("boostSpd", Bool, false, mutexBoosts));
        trainer.addChild(Node("boostSpc", Bool, false, mutexBoosts));


        auto pokemonDef = Node("", Object, false);
        auto pokemon = Node("pokemon", Array);

        pokemon.addChild(pokemonDef);

        specs.insert(trainer);
        specs.insert(pokemon);
    }

    bool isReady()
    {
        return readyFlag;
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
    connectLogger(nullptr);
}
