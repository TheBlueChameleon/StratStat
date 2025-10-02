#include <string>
using namespace std::string_literals;

#include "luaerrors.hpp"
#include "luatable.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaTable::LuaTable() {}

    void LuaTable::pushToLua(lua_State* L) const
    {

    }

    void LuaTable::addEntry(const KeyValuePair& entry)
    {
        auto copy = KeyValuePair(entry);
        table.emplace(std::move(copy));
        copy.setNull();
    }

    void LuaTable::addEntry(KeyValuePair&& entry)
    {
        table.emplace(std::move(entry));
        entry.setNull();
    }

    void LuaTable::addEntry(const LuaWrappable& key, const LuaWrappable& value)
    {
        auto entry = KeyValuePair(key, value);
        table.insert(std::move(entry));
        entry.setNull();
    }

    void LuaTable::addEntry(LuaWrappable&& key, LuaWrappable&& value)
    {
        auto&& entry = KeyValuePair(std::move(key), std::move(value));
        table.emplace(entry);
        entry.setNull();
    }

    const LuaWrappable& mapToKeys(const KeyValuePair& p)
    {
        return p.getKey();
    }

    bool LuaTable::hasKey(const LuaWrappable& key) const
    {
        auto matchesKey = [&key](const LuaWrappable& entryKey)
        {
            return entryKey == key;
        };

        auto it = std::ranges::find_if(table, matchesKey, mapToKeys);
        const auto notAvailable = table.end();
        return it != notAvailable;
    }

    const LuaWrappable& LuaTable::get(const LuaWrappable& key) const
    {
        auto matchesKey = [&key](const LuaWrappable& entryKey)
        {
            return entryKey == key;
        };

        auto it = std::ranges::find_if(table, matchesKey, mapToKeys);
        const auto notAvailable = table.end();
        if (it == notAvailable)
        {
            throw LuaError("Key '"s + key.getRepr() + "' was not found in the table.");
        }

        return it->getValue();
    }
}
