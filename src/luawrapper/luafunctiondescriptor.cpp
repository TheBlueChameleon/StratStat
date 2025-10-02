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
        for (int i = 0; const auto type : types)
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

    ParameterStack LuaFunctionDescriptor::fetchFromLua(lua_State* L, const std::vector<int>& types) const
    {
        ParameterStack result;

        for (const int typeID : std::ranges::reverse_view(types))
        {
            switch (typeID)
            {
                case LUA_TNIL:
                    result.push_front(nullptr);
                    break;
                case LUA_TBOOLEAN:
                    result.push_front(lua_toboolean(L, -1));
                    break;
                case LUA_TLIGHTUSERDATA:
                    throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
                    //result.push_front(lua_touserdata(L, -1));
                    break;
                case LUA_TNUMBER:
                    result.push_front(lua_tonumber(L, -1));
                    break;
                case LUA_TSTRING:
                    result.push_front(std::string(lua_tostring(L, -1)));
                    break;
                case LUA_TTABLE:
                    throw LuaError("Not yet implemented: Fetching type: " + getTypeName(typeID));
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
