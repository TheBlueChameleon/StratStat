#include <algorithm>
#include <iostream>
#include <string>
using namespace std::string_literals;

#include "luaerrors.hpp"
#include "luatable.hpp"
#include "luatableiterator.hpp"
#include "luatableconstiterator.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaTable::LuaTable() {}

    void LuaTable::pushToLua(lua_State* L) const
    {
        lua_newtable(L);

        for (const auto& [k, v] : *this)
        {
            std::cout << "key = " << k->getRepr() << "\tvalue = " << v->getRepr() << std::endl;
        }
        // lua_pushliteral(L, "key");
        // lua_pushliteral(L, "value");
        // lua_settable(L, -3);
    }

    size_t LuaTable::size() const
    {
        return table.size();
    }

    bool LuaTable::hasKey(const LuaWrappable& key) const
    {
        return table.contains(key);
    }

    using TableIterator = std::unordered_map<IndirectWrappable, IndirectWrappable>::iterator;
    using TableConstIterator = std::unordered_map<IndirectWrappable, IndirectWrappable>::const_iterator;

    std::pair<TableIterator, TableIterator> lookupKey(
        std::unordered_map<IndirectWrappable, IndirectWrappable>& table,
        const LuaWrappable& key)
    {
        auto it = table.find(key);
        const auto notAvailable = table.end();

        return {it, notAvailable};
    }

    std::pair<TableConstIterator, TableConstIterator> lookupKey(
        const std::unordered_map<IndirectWrappable, IndirectWrappable>& table,
        const LuaWrappable& key)
    {
        const auto it = table.find(key);
        const auto notAvailable = table.end();

        return {it, notAvailable};
    }

    const LuaWrappable& LuaTable::get(const LuaWrappable& key) const
    {
        const auto [it, notAvailable] = lookupKey(table, key);
        if (it == notAvailable)
        {
            throw LuaError("Key '"s + key.getRepr() + "' was not found in the table.");
        }
        return it->second.get();
    }

    const std::unordered_set<LuaWrappable> LuaTable::getKeySet() const
    {
        std::unordered_set<LuaWrappable> result;

        std::transform(table.begin(), table.end(),
                       std::inserter(result, result.end()),
                       [](const auto& pair)
        {
            return pair.first.get();
        }
                      );
        return result;
    }

    const std::unordered_multiset<LuaWrappable> LuaTable::getValues() const
    {
        std::unordered_multiset<LuaWrappable> result;
        std::transform(table.begin(), table.end(),
                       std::inserter(result, result.end()),
                       [](const auto& pair)
        {
            return pair.second.get();
        }
                      );
        return result;
    }

    void assertNoNullKey(const LuaWrappable& key)
    {
        if (key.isNil() || key.isError())
        {
            throw LuaError("attempting to create table entry with nil key");
        }
    }

    void LuaTable::setEntry(const LuaWrappable& key, const LuaWrappable& value)
    {
        assertNoNullKey(key);
        const auto [it, notAvailable] = lookupKey(table, key);
        if (it == notAvailable)
        {
            table.insert({key, value});
        }
        else
        {
            it->second.set(value);
        }

    }

    void LuaTable::setEntry(LuaWrappable&& key, LuaWrappable&& value)
    {
        assertNoNullKey(key);
        const auto [it, notAvailable] = lookupKey(table, key);
        if (it == notAvailable)
        {
            table.insert({std::move(key), std::move(value)});
        }
        else
        {
            it->second.set(std::move(value));
        }

    }

    LuaTableIterator LuaTable::begin()
    {
        return LuaTableIterator(table.begin());
    }

    LuaTableIterator LuaTable::end()
    {
        return LuaTableIterator(table.end());
    }

    LuaTableConstIterator LuaTable::begin() const
    {
        return LuaTableConstIterator(table.cbegin());
    }

    LuaTableConstIterator LuaTable::end() const
    {
        return LuaTableConstIterator(table.cend());
    }
}
