#ifndef LUASETOPERATIONS_HPP
#define LUASETOPERATIONS_HPP

#include <unordered_set>

namespace LuaWrapper
{
    class LuaFunctionDescriptor;
    class LuaWrappable;
    class KeyValuePair;

    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs);
    bool operator==(const KeyValuePair& lhs, const KeyValuePair& rhs);
    bool operator==(const LuaWrappable& lhs, const LuaWrappable& rhs);
    bool operator==(const KeyValuePair& lhs, const LuaWrappable& rhs);
}

namespace std
{
    template <> struct hash<LuaWrapper::LuaFunctionDescriptor>
    {
        size_t operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const;
    };

    template <> struct hash<LuaWrapper::KeyValuePair>
    {
        size_t operator()(const LuaWrapper::KeyValuePair& kvPair) const;
    };

    template <> struct hash<LuaWrapper::LuaWrappable>
    {
        size_t operator()(const LuaWrapper::LuaWrappable& wrappable) const;
    };
}

#endif // LUASETOPERATIONS_HPP
