#ifndef GEN1_CONSTANTS_HPP
#define GEN1_CONSTANTS_HPP

#include <unordered_set>
#include <string>
#include <vector>

#include "../sharedtypes.hpp"

// TODO fix capitalization rules

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

    constexpr auto STATUS_NONE = "OK";
    constexpr auto STATUS_SLEEP = "Sleep";
    constexpr auto STATUS_POISON = "Poison";
    constexpr auto STATUS_PARALYZED = "Paralyzed";
    constexpr auto STATUS_BURNED = "Burned";
    constexpr auto STATUS_FROZEN = "Frozen";

    extern const AllowedValues STATUS_ALL;

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
    constexpr auto TEAM_LEVEL = "level";
    constexpr auto TEAM_HP = "HP";
    constexpr auto TEAM_ATK = "ATK";
    constexpr auto TEAM_DEF = "DEF";
    constexpr auto TEAM_SPC = "SPC";
    constexpr auto TEAM_SPD = "SPD";
    constexpr auto TEAM_DV_ATK = "DV_ATK";
    constexpr auto TEAM_DV_DEF = "DV_DEF";
    constexpr auto TEAM_DV_SPC = "DV_SPC";
    constexpr auto TEAM_DV_SPD = "DV_SPD";
    constexpr auto TEAM_SX_HP = "StatExperience_HP";
    constexpr auto TEAM_SX_ATK = "StatExperience_ATK";
    constexpr auto TEAM_SX_DEF = "StatExperience_DEF";
    constexpr auto TEAM_SX_SPC = "StatExperience_SPC";
    constexpr auto TEAM_SX_SPD = "StatExperience_SPD";
    constexpr auto TEAM_MOVE1 = "move1";
    constexpr auto TEAM_MOVE2 = "move2";
    constexpr auto TEAM_MOVE3 = "move3";
    constexpr auto TEAM_MOVE4 = "move4";
    constexpr auto TEAM_MOVE1_PP = "move1PP";
    constexpr auto TEAM_MOVE2_PP = "move2PP";
    constexpr auto TEAM_MOVE3_PP = "move3PP";
    constexpr auto TEAM_MOVE4_PP = "move4PP";
    constexpr auto TEAM_MOVE1_MAX_PP = "move1MaxPP";
    constexpr auto TEAM_MOVE2_MAX_PP = "move2MaxPP";
    constexpr auto TEAM_MOVE3_MAX_PP = "move3MaxPP";
    constexpr auto TEAM_MOVE4_MAX_PP = "move4MaxPP";
    constexpr auto TEAM_STATUS = "status";
    constexpr auto TEAM_SLEEP_COUNTER = "sleepCounter";
    constexpr auto TEAM_TOXIC_COUNTER = "toxicCounter";
}

#endif // GEN1_CONSTANTS_HPP
