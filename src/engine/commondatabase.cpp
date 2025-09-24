#include "commondatabase.hpp"

namespace StratStat
{
    CommonDatabase::CommonDatabase() {}

    bool CommonDatabase::add(const CommonValueCollection& def)
    {
        auto [_, insertion] = db.insert({def.getIdentifier(), def});
        return insertion;
    }

    const CommonValueCollection& CommonDatabase::get(const std::string& identifier) const
    {
        return db.at(identifier);
    }

    const bool CommonDatabase::contains(const std::string& identifier) const
    {
        return db.contains(identifier);
    }

    int CommonDatabase::size() const
    {
        return db.size();
    }
}
