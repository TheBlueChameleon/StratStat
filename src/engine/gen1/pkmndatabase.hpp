#ifndef PKMNDATABASE_HPP
#define PKMNDATABASE_HPP

#include "../basepkmndatabase.hpp"
#include "pkmndef.hpp"

class PkmnDatabase : public BasePkmnDatabase
{
    public:
        PkmnDatabase();

        bool add(const PkmnDef& def);
        const PkmnDef& get(const std::string& identifier) const;
};

#endif // PKMNDATABASE_HPP
