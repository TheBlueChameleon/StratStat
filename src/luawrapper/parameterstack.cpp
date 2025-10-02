#include "luawrappable.hpp"
#include "parameterstack.hpp"

namespace LuaWrapper
{
    void ParameterStack::pushToLua(lua_State* L) const
    {
        for (const auto& param : *this)
        {
            param.pushToLua(L);
        }
    }
}
