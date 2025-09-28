#ifndef LUATABLE_HPP
#define LUATABLE_HPP

#include "lua.hpp"

namespace LuaWrapper
{
    class LuaWrappable;

    class LuaTable
    {
        private:
            // Collection<Pair<LW, LW>> entries

            void pushToLua(lua_State* L) const;

        public:
            LuaTable();

            friend class LuaWrappable;
    };
}

#endif // LUATABLE_HPP
