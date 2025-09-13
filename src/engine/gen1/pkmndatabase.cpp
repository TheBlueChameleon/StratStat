#include <utility>

#include "pkmndatabase.hpp"

PkmnDatabase::PkmnDatabase() {}

bool PkmnDatabase::add(const PkmnDef& def)
{
    return BasePkmnDatabase::add(def);
}

const PkmnDef& PkmnDatabase::get(const std::string& identifier) const
{
    return static_cast<const PkmnDef&>(BasePkmnDatabase::get(identifier));
}
