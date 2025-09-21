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

    extern const std::vector<VariantContentInfo> PKMN_DB_HEADERS;

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

    extern const std::vector<VariantContentInfo> MOVE_DB_HEADERS;

    constexpr auto PLAYER_HUMAN = "human";
    constexpr auto PLAYER_COMPUTER = "computer";
    constexpr auto PLAYER_BADGES = "badges";
    constexpr auto PLAYER_BADGE_BROCK = "Brock";
    constexpr auto PLAYER_BADGE_MISTY = "Misty";
    constexpr auto PLAYER_BADGE_SURGE = "Surge";
    constexpr auto PLAYER_BADGE_ERIKA = "Erika";
    constexpr auto PLAYER_BADGE_SABRINA = "Sabrina";
    constexpr auto PLAYER_BADGE_KOGA = "Koga";
    constexpr auto PLAYER_BADGE_BLAINE = "Blaine";
    constexpr auto PLAYER_BADGE_GIOVANNI = "Giovanni";
    constexpr auto PLAYER_POKEMON = "pokemon";
    constexpr auto PLAYER_BOOST_ATK = "boostAtk";
    constexpr auto PLAYER_BOOST_DEF = "boostDef";
    constexpr auto PLAYER_BOOST_SPC = "boostSpc";
    constexpr auto PLAYER_BOOST_SPD = "boostSpd";
    constexpr auto PLAYER_STATUS_MOVES_DEBUFF = "statMoveDebuf";
    constexpr auto PLAYER_USE_PP = "usePP";

    constexpr auto TEAM_SPECIES = "species";
    // ...
}

#endif // GEN1_CONSTANTS_HPP
