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

    size_t LuaTable::size() const
    {
        return table.size();
    }

    const LuaWrappable& mapToKeys(const KeyValuePair& p)
    {
        return p.getKey();
    }

    struct matchesKey
    {
        const LuaWrappable& key;

        bool operator()(const LuaWrappable& entryKey)
        {
            return entryKey == key;
        }
    };

    bool LuaTable::hasKey(const LuaWrappable& key) const
    {
        auto it = std::ranges::find_if(table, matchesKey(key), mapToKeys);
        const auto notAvailable = table.end();
        return it != notAvailable;
    }

    const LuaWrappable& LuaTable::get(const LuaWrappable& key) const
    {
        auto it = std::ranges::find_if(table, matchesKey(key), mapToKeys);
        const auto notAvailable = table.end();
        if (it == notAvailable)
        {
            throw LuaError("Key '"s + key.getRepr() + "' was not found in the table.");
        }

        return it->getValue();
    }

    void LuaTable::setEntry(const KeyValuePair& entry)
    {
        auto copy = KeyValuePair(entry);
        table.emplace(entry);
    }

    void LuaTable::setEntry(KeyValuePair&& entry)
    {
        table.emplace(std::move(entry));
    }

    void LuaTable::setEntry(const LuaWrappable& key, const LuaWrappable& value)
    {
        table.insert(KeyValuePair(key, value));
    }

    void LuaTable::setEntry(LuaWrappable&& key, LuaWrappable&& value)
    {
        setEntry(KeyValuePair(std::move(key), std::move(value)));
    }
}
