#ifndef BASEPKMNDATABASE_H
#define BASEPKMNDATABASE_H

#include <map>
#include <string>

#include "basepkmndef.hpp"

class BasePkmnDatabase
{
    private:
        std::map<std::string, BasePkmnDef> db;

    public:
        BasePkmnDatabase();


        bool add(const BasePkmnDef& def);
};

#endif // BASEPKMNDATABASE_H
