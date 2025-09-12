#ifndef PKMNDATABASE_HPP
#define PKMNDATABASE_HPP

#include "../basepkmndatabase.hpp"
#include "pkmndef.hpp"

class PkmnDatabase : public BasePkmnDatabase
{
    public:
        PkmnDatabase();

        bool add(const PkmnDef& def);
};

#endif // PKMNDATABASE_HPP
