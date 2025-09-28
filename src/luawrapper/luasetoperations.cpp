#include <string>

#include "luafunctiondescriptor.hpp"
#include "luasetoperations.hpp"

namespace LuaWrapper
{
    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs)
    {
        return lhs.getFuncName() == rhs.getFuncName();
    }
}

namespace std
{
    size_t hash<LuaWrapper::LuaFunctionDescriptor>::operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const
    {
        return std::hash<std::string>()(funcDescriptor.getFuncName());
    }
}
