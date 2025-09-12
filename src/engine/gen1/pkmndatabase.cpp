#include "pkmndatabase.hpp"

PkmnDatabase::PkmnDatabase() {}

bool PkmnDatabase::add(const PkmnDef& def)
{
    return BasePkmnDatabase::add(def);
}
