#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
using namespace std::string_literals;

#include "luacapi.hpp"
#include "luautils.hpp"
#include "luaerrors.hpp"
#include "luafunctiondescriptor.hpp"
#include "luawrappable.hpp"
#include "parameterstack.hpp"

namespace LuaWrapper
{
    void assertTypeRange(const std::vector<int>& types)
    {
        int i = 0;
        for (const auto type : types)
        {
            if ((type <= LUA_TNONE) || (type >= LUA_NUMTYPES))
            {
                throw LuaError("Unknown type identifier: "s + std::to_string(type) + " in parameter #" +  std::to_string(i));
            }
            ++i;
        }
    }

    LuaFunctionDescriptor::LuaFunctionDescriptor(
        const std::string& funcName,
        const std::vector<int>& inParamTypes,
        const std::vector<int>& outParamTypes
    ) :
        funcName(funcName),
        inputParamTypes(inParamTypes),
        outputParamTypes(outParamTypes)
    {
        assertTypeRange(inParamTypes);
        assertTypeRange(outParamTypes);
    }

    LuaWrappable fetchTypeIDFromLua(lua_State* L, const int typeID);

    LuaTable fetchTableFromLua(lua_State* L)
    {
        LuaTable result;

        lua_pushnil(L);  /* Make sure lua_next starts at beginning */
        LuaWrappable key, value;
        while (lua_next(L, -2))                      /* TABLE LOCATED AT -2 IN STACK */
        {
            value = fetchTypeIDFromLua(L, lua_type(L, -1));
            lua_pop(L,1);
            key = fetchTypeIDFromLua(L, lua_type(L, -1));
            result.setEntry(std::move(key), std::move(value));
        }

        return result;
    }

    LuaWrappable fetchTypeIDFromLua(lua_State* L, const int typeID)
    {
        switch (typeID)
        {
            case LUA_TNIL:
                return nullptr;
            case LUA_TBOOLEAN:
                return lua_toboolean(L, -1);
            case LUA_TLIGHTUSERDATA:
                //return lua_touserdata(L, -1);
                throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
            case LUA_TNUMBER:
                return lua_tonumber(L, -1);
            case LUA_TSTRING:
                return std::string(lua_tostring(L, -1));
            case LUA_TTABLE:
                return fetchTableFromLua(L);
            case LUA_TFUNCTION:
                throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
            case LUA_TUSERDATA:
                throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
            case LUA_TTHREAD:
                throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
            default:
                throw LuaError("Unknown typeID: "s + std::to_string(typeID));
        }

        lua_pop(L, 1);
    }

    ParameterStack LuaFunctionDescriptor::fetchFromLua(lua_State* L, const std::vector<int>& types) const
    {
        ParameterStack result;

        for (const int typeID : std::ranges::reverse_view(types))
        {
            result.push_front(fetchTypeIDFromLua(L, typeID));
        }

        return result;
    }

    const std::string& LuaFunctionDescriptor::getFuncName() const
    {
        return funcName;
    }

    void LuaFunctionDescriptor::setFuncName(const std::string& newFuncName)
    {
        funcName = newFuncName;
    }

    const std::vector<int>& LuaFunctionDescriptor::getInputParamTypes() const
    {
        return inputParamTypes;
    }

    void LuaFunctionDescriptor::setInputParamTypes(const std::vector<int>& newInputParamTypes)
    {
        assertTypeRange(newInputParamTypes);
        inputParamTypes = newInputParamTypes;
    }

    const std::vector<int>& LuaFunctionDescriptor::getOutputParamTypes() const
    {
        return outputParamTypes;
    }

    void LuaFunctionDescriptor::setOutputParamTypes(const std::vector<int>& newOutputParamTypes)
    {
        assertTypeRange(newOutputParamTypes);
        outputParamTypes = newOutputParamTypes;
    }
}
