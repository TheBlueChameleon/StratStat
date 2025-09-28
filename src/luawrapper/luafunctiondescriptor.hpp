#ifndef LUAFUNCTIONDESCRIPTOR_H
#define LUAFUNCTIONDESCRIPTOR_H

#include <list>
#include <string>
#include <vector>

#include "lua.hpp"

namespace LuaWrapper
{
    class ParameterStack;

    class LuaFunctionDescriptor
    {
        private:
            std::string funcName;
            std::vector<int> inputParamTypes;
            std::vector<int> outputParamTypes;

            ParameterStack fetchFromLua(lua_State* L, const std::vector<int>& types) const;

        public:
            LuaFunctionDescriptor(
                const std::string& funcName,
                const std::vector<int>& inParamTypes = {},
                const std::vector<int>& outParamTypes = {}
            );

            const std::string& getFuncName() const;
            void setFuncName(const std::string& newFuncName);

            const std::vector<int>& getInputParamTypes() const;
            void setInputParamTypes(const std::vector<int>& newInputParamTypes);

            const std::vector<int>& getOutputParamTypes() const;
            void setOutputParamTypes(const std::vector<int>& newOutputParamTypes);

            friend class State;
    };

    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs);
}

namespace std
{
    template <> struct hash<LuaWrapper::LuaFunctionDescriptor>
    {
        size_t operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const;
    };
}

#endif // LUAFUNCTIONDESCRIPTOR_H
