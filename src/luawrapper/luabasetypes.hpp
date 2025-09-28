#ifndef LUABASETYPES_HPP
#define LUABASETYPES_HPP

#include <string>
#include <variant>

#include "luafunctiondescriptor.hpp"
#include "luatable.hpp"

namespace LuaWrapper
{
    using LuaWrappableVariant = std::variant<nullptr_t, bool, void*, double, std::string, LuaTable, LuaFunctionDescriptor, std::monostate>;

    std::string getTypeName(const int typeID);
}

#endif // LUABASETYPES_HPP
