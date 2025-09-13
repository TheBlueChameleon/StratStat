#include "movedatabase.hpp"

MoveDatabase::MoveDatabase() {}

bool MoveDatabase::add(const MoveDef& def)
{
    return BaseMoveDatabase::add(def);
}

const MoveDef& MoveDatabase::get(const std::string& identifier) const
{
    return static_cast<const MoveDef&>(BaseMoveDatabase::get(identifier));
}
