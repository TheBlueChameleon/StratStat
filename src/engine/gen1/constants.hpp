#ifndef GEN1_CONSTANTS_HPP
#define GEN1_CONSTANTS_HPP

#include <unordered_set>
#include <string>
#include <vector>

#include "../sharedtypes.hpp"

namespace StratStat
{
    constexpr auto PKMN_IDENTIFIER = "Name";
    constexpr auto PKMN_TYPE1 = "Type1";
    constexpr auto PKMN_TYPE2 = "Type2";
    constexpr auto PKMN_HP = "HP";
    constexpr auto PKMN_ATK = "ATK";
    constexpr auto PKMN_DEF = "DEF";
    constexpr auto PKMN_SPC = "SPC";
    constexpr auto PKMN_SPD = "SPD";
    constexpr auto PKMN_EXPGROUP = "EXPGROUP";

    extern const AllowedValues PKMN_TYPES;
    extern const AllowedValues PKMN_EXPERIENCE_GROUPS;

    constexpr auto PKMN_DB_HEADERS =
    {
        VariantContentInfo(PKMN_IDENTIFIER,         VariantContentID::Text),
        VariantContentInfo(PKMN_TYPE1,              VariantContentID::Text),
        VariantContentInfo(PKMN_TYPE2,              VariantContentID::Text),
        VariantContentInfo(PKMN_HP,                 VariantContentID::Integer),
        VariantContentInfo(PKMN_ATK,                VariantContentID::Integer),
        VariantContentInfo(PKMN_DEF,                VariantContentID::Integer),
        VariantContentInfo(PKMN_SPC,                VariantContentID::Integer),
        VariantContentInfo(PKMN_SPD,                VariantContentID::Integer),
        VariantContentInfo(PKMN_EXPGROUP,           VariantContentID::Text),
    };

    constexpr auto MOVE_IDENTIFIER = "identifier";
    constexpr auto MOVE_TYPE_ID = "type_id";
    constexpr auto MOVE_POWER = "power";
    constexpr auto MOVE_PP = "pp";
    constexpr auto MOVE_ACCURACY = "accuracy";
    constexpr auto MOVE_PRIORITY = "priority";
    constexpr auto MOVE_DAMAGE_CLASS_ID = "damage_class_id";
    constexpr auto MOVE_EFFECT_ID = "effect_id";
    constexpr auto MOVE_EFFECT_CHANCE = "effect_chance";

    extern const AllowedValues MOVE_DAMAGE_CLASSIDS;
    extern const AllowedValues MOVE_EFFECT_IDS;

    constexpr auto MOVE_DB_HEADERS =
    {
        VariantContentInfo(MOVE_IDENTIFIER,         VariantContentID::Text),
        VariantContentInfo(MOVE_TYPE_ID,            VariantContentID::Text),
        VariantContentInfo(MOVE_POWER,              VariantContentID::Integer),
        VariantContentInfo(MOVE_PP,                 VariantContentID::Integer),
        VariantContentInfo(MOVE_ACCURACY,           VariantContentID::Integer),
        VariantContentInfo(MOVE_PRIORITY,           VariantContentID::Integer),
        VariantContentInfo(MOVE_DAMAGE_CLASS_ID,    VariantContentID::Text),
        VariantContentInfo(MOVE_EFFECT_ID,          VariantContentID::Text),
        VariantContentInfo(MOVE_EFFECT_CHANCE,      VariantContentID::Integer),
    };
}

#endif // GEN1_CONSTANTS_HPP
