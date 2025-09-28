#ifndef LUASETOPERATIONS_HPP
#define LUASETOPERATIONS_HPP

#include <unordered_set>

namespace LuaWrapper
{
    class LuaFunctionDescriptor;
    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs);
}

namespace std
{
    template <> struct hash<LuaWrapper::LuaFunctionDescriptor>
    {
        size_t operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const;
    };
}

#endif // LUASETOPERATIONS_HPP
