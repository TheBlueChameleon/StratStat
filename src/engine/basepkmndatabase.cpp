#include "basepkmndatabase.hpp"

BasePkmnDatabase::BasePkmnDatabase() {}

bool BasePkmnDatabase::add(const BasePkmnDef& def)
{
    auto [_, insertion] = db.insert({def.getIdentifier(), def});
    return insertion;
}

const BasePkmnDef& BasePkmnDatabase::get(const std::string& identifier) const
{
    return db.at(identifier);
}

int BasePkmnDatabase::size() const
{
    return db.size();
}
