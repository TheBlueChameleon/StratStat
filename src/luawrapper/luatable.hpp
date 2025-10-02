#ifndef LUATABLE_HPP
#define LUATABLE_HPP

#include <unordered_set>
#include <vector>

#include "luacapi.hpp"
#include "luasetoperations.hpp"
#include "keyvaluepair.hpp"

namespace LuaWrapper
{
    class LuaWrappable;

    class LuaTable
    {
        private:
            std::unordered_set<KeyValuePair> table;

            void pushToLua(lua_State* L) const;

        public:
            LuaTable();

            void addEntry(const KeyValuePair& entry);
            void addEntry(KeyValuePair&& entry);
            void addEntry(const LuaWrappable& key, const LuaWrappable& value);
            void addEntry(LuaWrappable&& key, LuaWrappable&& value);

            bool hasKey(const LuaWrappable& key) const;
            const LuaWrappable &get(const LuaWrappable& key) const;

            friend class LuaWrappable;
    };
}

#endif // LUATABLE_HPP
