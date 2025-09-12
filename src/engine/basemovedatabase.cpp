#include "basemovedatabase.hpp"

BaseMoveDatabase::BaseMoveDatabase() {}

bool BaseMoveDatabase::add(const BaseMoveDef& def)
{
    auto [_, insertion] = db.insert({def.getIdentifier(), def});
    return insertion;
}

int BaseMoveDatabase::size() const
{
    return db.size();
}
