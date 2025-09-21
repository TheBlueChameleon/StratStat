#ifndef COMMONDATABASE_HPP
#define COMMONDATABASE_HPP

#include <unordered_map>

#include "commonvaluecollection.hpp"

namespace StratStat
{
    class CommonDatabase
    {
        private:
            std::unordered_map<std::string, CommonValueCollection> db;

        public:
            CommonDatabase();

            bool add(const CommonValueCollection& def);
            const CommonValueCollection& get(const std::string& identifier) const;

            int size() const;
    };
}

#endif // COMMONDATABASE_HPP
