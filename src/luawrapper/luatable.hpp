#ifndef LUATABLE_HPP
#define LUATABLE_HPP

#include "lua.hpp"

namespace LuaWrapper
{
    class LuaTable
    {
        public:
            LuaTable();

            void pushToLua(lua_State* L) const;
    };
}

#endif // LUATABLE_HPP
