#include <iostream>
#include <string>
#include <vector>
using namespace std::string_literals;

#include <app/errors.hpp>

#include "luacapi.hpp"
#include "luautils.hpp"
#include "luaerrors.hpp"
#include "luawrappable.hpp"
#include "parameterstack.hpp"
#include "luastate.hpp"

namespace LuaWrapper
{
    LuaState::LuaState(const std::filesystem::path& scriptFile)
    {
        initLua(scriptFile);
        scanForLuaFunctions();
    }

    LuaState::~LuaState()
    {
        if (L)
        {
            lua_close(L);
        }
        L = nullptr;
    }

    void LuaState::initLua(const std::filesystem::path& scriptFile)
    {
        L = luaL_newstate();
        luaL_openlibs(L);

        if (luaL_loadfile(L, scriptFile.c_str()))
        {
            throw CriticalAbort(
                "Could not load script '"s + scriptFile.c_str() + "':\n" +
                lua_tostring(L, -1)
            );
        }

        // Priming call. Without this, function names in script are unknown
        if (lua_pcall(L, 0, 0, 0))
        {
            throw CriticalAbort(
                "Could not initialize script '"s + scriptFile.c_str() + "'.\n" +
                lua_tostring(L, -1)
            );
        }
    }

    void LuaState::scanForLuaFunctions()
    {
        const char* name;

        lua_pushglobaltable(L);                 // stack = [table]
        lua_pushnil(L);                         // stack = [nil, table]
        while (lua_next(L, -2) != 0)            // stack = [<next value>, <next key>, table]
        {
            // lua_next implies lua_pop(L, 1) to get key
            if (lua_isfunction(L, -1))
            {
                name = lua_tostring(L, -2);     // Get key(-2)
                luaFunctionNames.emplace(name);
            }
            lua_pop(L, 1);                      // stack = [<next key>, table]
        }
        // lua_next has exhausted table         => stack = [table]
        lua_pop(L, 1);                          // stack = []
    }

    void LuaState::verifyParameterStack(const LuaFunctionDescriptor& fDescriptor, const ParameterStack& parameters)
    {
        const std::vector<int>& expectedTypes = fDescriptor.getInputParamTypes();

        if (expectedTypes.size() != parameters.size())
        {
            throw LuaError(
                "Error when calling the function '" + fDescriptor.getFuncName() + "': "
                "Expected " + std::to_string(expectedTypes.size()) + " parameters, "
                "but got " + std::to_string(parameters.size()) + " parameters."
            );
        }

        for (int i = 0; i < expectedTypes.size(); ++i)
        {
            const auto actualType_i = parameters[i].getType();
            if (expectedTypes[i] != actualType_i)
            {
                throw LuaError(
                    "Error when calling the function '" + fDescriptor.getFuncName() + "': "
                    "Argument type mismatch in parameter #" + std::to_string(i) + ". "
                    "Expected " + getTypeName(expectedTypes[i]) + ", "
                    "but got " + getTypeName(actualType_i) + " parameters."
                );
            }
        }
    }

    const std::unordered_set<std::string>& LuaState::getNativeFunctions() const
    {
        return luaFunctionNames;
    }

    const LuaFunctionDescriptor& LuaState::getDescriptorForName(const std::string& name) const
    {
        const auto notAvailable = registeredFunctions.end();
        const auto it = registeredFunctions.find(name);
        if (it == notAvailable)
        {
            throw LuaError("The function '" + name + "' was not registered in the LuaWrapper");
        }
        return *it;
    }

    void LuaState::registerLuaFunction(const LuaFunctionDescriptor& functionDescriptor)
    {
        if (!luaFunctionNames.contains(functionDescriptor.getFuncName()))
        {
            throw LuaError("No function with name '"s + functionDescriptor.getFuncName() + "' was found in the script.");
        }

        const auto [it, insertionHappened] = registeredFunctions.insert(functionDescriptor);

        if (!insertionHappened)
        {
            throw LuaError("A function with name '"s + functionDescriptor.getFuncName() + "' has already been registered.");
        }
    }

    void LuaState::registerCFunction(const std::string& name, void* fnPtr)
    {

    }

    const ParameterStack LuaState::invoke(const std::string& name, const ParameterStack& parameters)
    {
        const LuaFunctionDescriptor& fDescriptor = getDescriptorForName(name);
        verifyParameterStack(fDescriptor, parameters);

        lua_getglobal(L, name.c_str());
        parameters.pushToLua(L);
        if (lua_pcall(L, parameters.size(), fDescriptor.getOutputParamTypes().size(), 0) != LUA_OK)
        {
            throw LuaError(
                "Call to '" + name + "' failed: " +
                lua_tostring(L, -1)
            );
        }

        ParameterStack result = fDescriptor.fetchFromLua(L, fDescriptor.getOutputParamTypes());
        return result;
    }
}
