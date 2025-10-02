#ifndef LUASTATE_H
#define LUASTATE_H

#include <filesystem>
#include <list>
#include <unordered_set>

#include "luacapi.hpp"
#include "luasetoperations.hpp"

namespace LuaWrapper
{
    class LuaFunctionDescriptor;
    class ParameterStack;

    class LuaState
    {
        private:
            lua_State* L = nullptr;
            std::unordered_set<std::string>             cFunctionNames;
            std::unordered_set<std::string>             luaFunctionNames;
            std::unordered_set<LuaFunctionDescriptor>   registeredFunctions;

            void initLua(const std::filesystem::path& scriptFile);
            void scanForLuaFunctions();
            void verifyParameterStack(const LuaFunctionDescriptor& fDescriptor, const ParameterStack& parameters);

        public:
            LuaState(const std::filesystem::path& scriptFile);
            ~LuaState();

            const std::unordered_set<std::string>& getNativeFunctions() const;
            const LuaFunctionDescriptor& getDescriptorForName(const std::string& name) const;

            void registerLuaFunction(const LuaFunctionDescriptor& functionDescriptor);
            void registerCFunction(const std::string& name, void* fnPtr);

            const ParameterStack invoke(const std::string& name, const ParameterStack& parameters);
    };
}

#endif // LUASTATE_H
