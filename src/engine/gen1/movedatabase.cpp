#include "movedatabase.hpp"

MoveDatabase::MoveDatabase() {}

bool MoveDatabase::add(const MoveDef& def)
{
    return BaseMoveDatabase::add(def);
}
