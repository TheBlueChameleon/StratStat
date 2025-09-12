#include "basepkmndatabase.hpp"

BasePkmnDatabase::BasePkmnDatabase() {}

bool BasePkmnDatabase::add(const BasePkmnDef& def)
{
    auto [_, insertion] = db.insert({def.getIdentifier(), def});
    return insertion;
}

int BasePkmnDatabase::size() const
{
    return db.size();
}
