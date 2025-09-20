#ifndef GEN1_CONSTANTS_HPP
#define GEN1_CONSTANTS_HPP

#include <initializer_list>
#include <string>

#include "../sharedtypes.hpp"

constexpr auto JSON_MANDATORY = "MANDATORY";
constexpr auto JSON_TYPE = "TYPE";
constexpr auto JSON_OBJECT = "OBJECT";
constexpr auto JSON_LIST = "LIST";
constexpr auto JSON_STRING = "STRING";
constexpr auto JSON_INTEGER = "INTEGER";

constexpr auto JSON_TRAINER = "Trainer";

constexpr auto PKMN_IDENTIFIER = "Name";
constexpr auto PKMN_TYPE1 = "Type1";
constexpr auto PKMN_TYPE2 = "Type2";
constexpr auto PKMN_HP = "HP";
constexpr auto PKMN_ATK = "ATK";
constexpr auto PKMN_DEF = "DEF";
constexpr auto PKMN_SPC = "SPC";
constexpr auto PKMN_SPD = "SPD";
constexpr auto PKMN_EXPGROUP = "EXPGROUP";

constexpr auto MOVE_IDENTIFIER = "identifier";

constexpr std::initializer_list<std::string> PKMN_TYPES =
{
    "Bug", "Dragon", "Electric", "Fighting", "Fire",
    "Flying", "Ghost", "Grass", "Ground", "Ice",
    "Normal", "Poison", "Psychic", "Rock", "Water"
};

constexpr auto PKMN_DB_HEADERS =
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

constexpr auto MOVE_DB_HEADERS =
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

#endif // GEN1_CONSTANTS_HPP
