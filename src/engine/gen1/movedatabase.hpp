#ifndef MOVEDATABASE_H
#define MOVEDATABASE_H

#include "../basemovedatabase.hpp"

#include "movedef.hpp"

class MoveDatabase : public BaseMoveDatabase
{
    public:
        MoveDatabase();

        bool add(const MoveDef& def);
        const MoveDef& get(const std::string& identifier) const;
};

#endif // MOVEDATABASE_H
