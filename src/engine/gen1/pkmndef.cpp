#include <iostream>

#include "constants.hpp"
#include "pkmndef.hpp"

PkmnDef::PkmnDef(const std::unordered_map<std::string, VariantContentType>& data) :
    BasePkmnDef(PKMN_IDENTIFIER, data)
{}

PkmnDef::PkmnDef(const BasePkmnDef& data) :
    BasePkmnDef(data)
{}

const std::string& PkmnDef::getName() const
{
    return std::get<static_cast<int>(VariantContentID::Text)>(
               BasePkmnDef::get(PKMN_IDENTIFIER)
           );
}


const std::string& PkmnDef::getType1() const
{
    return std::get<static_cast<int>(VariantContentID::Text)>(
               BasePkmnDef::get(PKMN_TYPE1)
           );
}

const std::string& PkmnDef::getType2() const
{
    return std::get<static_cast<int>(VariantContentID::Text)>(
               BasePkmnDef::get(PKMN_TYPE2)
           );
}

int PkmnDef::getHP() const
{
    return std::get<static_cast<int>(VariantContentID::Integer)>(
               BasePkmnDef::get(PKMN_HP)
           );
}

int PkmnDef::getATK() const
{
    return std::get<static_cast<int>(VariantContentID::Integer)>(
               BasePkmnDef::get(PKMN_ATK)
           );
}

int PkmnDef::getDEF() const
{
    return std::get<static_cast<int>(VariantContentID::Integer)>(
               BasePkmnDef::get(PKMN_DEF)
           );
}

int PkmnDef::getSPC() const
{
    return std::get<static_cast<int>(VariantContentID::Integer)>(
               BasePkmnDef::get(PKMN_SPC)
           );
}

int PkmnDef::getSPD() const
{
    return std::get<static_cast<int>(VariantContentID::Integer)>(
               BasePkmnDef::get(PKMN_SPD)
           );
}
