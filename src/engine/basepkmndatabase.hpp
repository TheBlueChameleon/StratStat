#ifndef BASEPKMNDATABASE_H
#define BASEPKMNDATABASE_H

#include <unordered_map>
#include <string>

#include "basepkmndef.hpp"

class BasePkmnDatabase
{
    private:
        std::unordered_map<std::string, BasePkmnDef> db;

    public:
        BasePkmnDatabase();

        bool add(const BasePkmnDef& def);
        const BasePkmnDef& get(const std::string& identifier) const;

        int size() const;
};

#endif // BASEPKMNDATABASE_H
