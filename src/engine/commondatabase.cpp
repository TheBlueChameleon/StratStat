#include "commondatabase.hpp"

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

int CommonDatabase::size() const
{
    return db.size();
}
