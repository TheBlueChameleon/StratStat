#ifndef BASEMOVEDATABASE_H
#define BASEMOVEDATABASE_H

#include <string>
#include <unordered_map>

#include "basemovedef.hpp"

class BaseMoveDatabase
{
    private:
        std::unordered_map<std::string, BaseMoveDef> db;

    public:
        BaseMoveDatabase();

        bool add(const BaseMoveDef& def);

        int size() const;
};

#endif // BASEMOVEDATABASE_H
