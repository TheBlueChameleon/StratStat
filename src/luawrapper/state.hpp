#ifndef STATE_H
#define STATE_H

#include <filesystem>
#include <list>
#include <unordered_set>

#include <lua.hpp>
#include "luafunctiondescriptor.hpp"
#include "parameterstack.hpp"

namespace LuaWrapper
{
    class LuaFunctionDescriptor;
    class ParameterStack;

    class State
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
            State(const std::filesystem::path& scriptFile);
            ~State();

            const std::unordered_set<std::string>& getNativeFunctions() const;
            const LuaFunctionDescriptor& getDescriptorForName(const std::string& name) const;

            void registerLuaFunction(const LuaFunctionDescriptor& functionDescriptor);
            void registerCFunction(const std::string& name, void* fnPtr);

            const ParameterStack invoke(const std::string& name, const ParameterStack& parameters);
    };
}

#endif // STATE_H
