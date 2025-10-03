#ifndef LUATABLE_HPP
#define LUATABLE_HPP

#include <unordered_set>

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

            size_t size() const;
            bool hasKey(const LuaWrappable& key) const;
            const LuaWrappable& get(const LuaWrappable& key) const;

            void setEntry(const KeyValuePair& entry);
            void setEntry(KeyValuePair&& entry);
            void setEntry(const LuaWrappable& key, const LuaWrappable& value);
            void setEntry(LuaWrappable&& key, LuaWrappable&& value);


            friend class LuaWrappable;
    };
}

#endif // LUATABLE_HPP
