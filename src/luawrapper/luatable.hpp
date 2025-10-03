#ifndef LUATABLE_HPP
#define LUATABLE_HPP

#include <unordered_set>
#include <unordered_map>

#include "indirectwrappable.hpp"
#include "luacapi.hpp"
#include "luasetoperations.hpp"
#include "luatableiterator.hpp"
#include "luatableconstiterator.hpp"

namespace LuaWrapper
{
    class LuaWrappable;

    class LuaTable
    {
        private:
            std::unordered_map<IndirectWrappable, IndirectWrappable> table;

            void pushToLua(lua_State* L) const;

        public:
            LuaTable();

            size_t size() const;
            bool hasKey(const LuaWrappable& key) const;
            const LuaWrappable& get(const LuaWrappable& key) const;
            const std::unordered_set<LuaWrappable> getKeySet() const;
            const std::unordered_multiset<LuaWrappable> getValues() const;

            void setEntry(const LuaWrappable& key, const LuaWrappable& value);
            void setEntry(LuaWrappable&& key, LuaWrappable&& value);

            LuaTableIterator begin();
            LuaTableIterator end();

            LuaTableConstIterator begin() const;
            LuaTableConstIterator end() const;

            bool isEqualTo(const LuaTable& other) const;

            friend class LuaWrappable;
    };
}

#endif // LUATABLE_HPP
