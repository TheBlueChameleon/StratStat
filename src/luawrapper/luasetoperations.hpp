#ifndef LUASETOPERATIONS_HPP
#define LUASETOPERATIONS_HPP

#include <unordered_set>

namespace LuaWrapper
{
    class IndirectWrappable;
    class LuaFunctionDescriptor;
    class LuaWrappable;

    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs);
    bool operator==(const LuaWrappable& lhs, const LuaWrappable& rhs);
    bool operator==(const IndirectWrappable& lhs, const IndirectWrappable& rhs);
}

namespace std
{
    template <> struct hash<LuaWrapper::LuaFunctionDescriptor>
    {
        size_t operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const;
    };

    template <> struct hash<LuaWrapper::LuaWrappable>
    {
        size_t operator()(const LuaWrapper::LuaWrappable& wrappable) const;
    };

    template <> struct hash<LuaWrapper::IndirectWrappable>
    {
        size_t operator()(const LuaWrapper::IndirectWrappable& wrappable) const;
    };
}

#endif // LUASETOPERATIONS_HPP
