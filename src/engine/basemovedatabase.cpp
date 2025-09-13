#include "basemovedatabase.hpp"

BaseMoveDatabase::BaseMoveDatabase() {}

bool BaseMoveDatabase::add(const BaseMoveDef& def)
{
    auto [_, insertion] = db.insert({def.getIdentifier(), def});
    return insertion;
}

const BaseMoveDef& BaseMoveDatabase::get(const std::string& identifier) const
{
    return db.at(identifier);
}

int BaseMoveDatabase::size() const
{
    return db.size();
}
