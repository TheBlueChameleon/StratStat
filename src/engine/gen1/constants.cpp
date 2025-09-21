#include "constants.hpp"

namespace StratStat
{
    const AllowedValues PKMN_TYPES =
    {
        "Bug", "Dragon", "Electric", "Fighting", "Fire",
        "Flying", "Ghost", "Grass", "Ground", "Ice",
        "Normal", "Poison", "Psychic", "Rock", "Water"
    };

    const AllowedValues PKMN_EXPERIENCE_GROUPS =
    {
        "Medium Fast",
        "Medium Slow",
        "Fast",
        "Slow"
    };

    const AllowedValues MOVE_DAMAGE_CLASSIDS = {"PHYSICAL", "SPECIAL"};
    const AllowedValues MOVE_EFFECT_IDS =
    {
        "BIDE",
        "BYPASS_HIT_CHECK",
        "CONVERSION",
        "DEFAULT",
        "DRAIN",
        "HIGH_CRIT",
        "MIRROR_LAST",
        "RANDOM_MOVE_GLOBAL",
        "REPEAT_2",
        "REPEAT_2_3_AND_USR_CONFUSE",
        "REPEAT_2_5",
        "REPEAT_2_AND_TRG_POISON_%",
        "RESET_ALL_STATS",
        "STRUGGLE",
        "TRG_20",
        "TRG_40",
        "TRG_BURN_%",
        "TRG_CONFUSE",
        "TRG_CONFUSE_%",
        "TRG_DISABLE_1_8",
        "TRG_DMG_DEPENDS_LAST_DMG",
        "TRG_DMG_EQUALS_USR_LEVEL",
        "TRG_DMG_RANDOM_USR_LEVEL",
        "TRG_FAINT",
        "TRG_FLINCH_%",
        "TRG_FREEZE_%",
        "TRG_HALF_HP",
        "TRG_LEECH",
        "TRG_LOWER_ACC",
        "TRG_LOWER_ATK",
        "TRG_LOWER_ATK_%",
        "TRG_LOWER_DEF",
        "TRG_LOWER_DEF_2",
        "TRG_LOWER_SPD_%",
        "TRG_LOWER_SPE_%",
        "TRG_LOWER_SPE_2",
        "TRG_PARALYZE",
        "TRG_PARALYZE_%",
        "TRG_POISON",
        "TRG_POISON_%",
        "TRG_REQ_SLEEP",
        "TRG_SLEEP",
        "TRG_SWITCH",
        "TRG_TOXIC",
        "TRG_TRAP_2_5",
        "TRG_TRI_%",
        "USR_CHARGE_BEFORE",
        "USR_CHARGE_BEFORE_AND_FLINCH_TRG_%",
        "USR_COPY_LAST_TRG_MOVE",
        "USR_FAINT",
        "USR_FLY",
        "USR_HEAL_AND_SLEEP_2",
        "USR_PROTECT_STATS",
        "USR_RAISE_ATK",
        "USR_RAISE_ATK_2",
        "USR_RAISE_ATK_IF_HIT",
        "USR_RAISE_CRIT",
        "USR_RAISE_DEF",
        "USR_RAISE_DEF_2",
        "USR_RAISE_EV",
        "USR_RAISE_SPC",
        "USR_RAISE_SPD_2",
        "USR_RAISE_SPE_2",
        "USR_RECHARGE_AFTER",
        "USR_RECOIL_2_ON_FAIL",
        "USR_RECOIL_4",
        "USR_RECOVER_50",
        "USR_SHIELD_PHY",
        "USR_SHIELD_SPC",
        "USR_SUBSTITUTE",
        "USR_TRANSFORM",
        "USR_UNDERGROUND"
    };
}
