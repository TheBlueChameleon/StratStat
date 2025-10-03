#include <string>
using namespace std::string_literals;

#include "luaerrors.hpp"
#include "luafunctiondescriptor.hpp"
#include "luasetoperations.hpp"
#include "luautils.hpp"
#include "luawrappable.hpp"
#include "keyvaluepair.hpp"

namespace LuaWrapper
{
    bool operator==(const LuaFunctionDescriptor& lhs, const LuaFunctionDescriptor& rhs)
    {
        return lhs.getFuncName() == rhs.getFuncName();
    }

    bool operator==(const KeyValuePair& lhs, const KeyValuePair& rhs)
    {
        return lhs.getKey() == rhs.getKey();
    }

    bool operator==(const LuaWrappable& lhs, const LuaWrappable& rhs)
    {
        if (lhs.getType() != rhs.getType())
        {
            return false;
        }

        switch (lhs.getType())
        {
            case LUA_TNIL:
                return true;
            case LUA_TBOOLEAN:
                return lhs.getBool() == rhs.getBool();
            case LUA_TLIGHTUSERDATA:
                return lhs.getPtr() == rhs.getPtr();
            case LUA_TNUMBER:
                return lhs.getDouble() == rhs.getDouble();
            case LUA_TSTRING:
                return lhs.getString() == rhs.getString();
            case LUA_TTABLE:
                return lhs.getTable() == rhs.getTable();
            case LUA_TFUNCTION:
                throw LuaError("Not Implemented yet");
            default:
                throw LuaError("Unknown typeID: "s + std::to_string(lhs.getType()));
        }
    }
}

namespace std
{
    size_t hash<LuaWrapper::LuaFunctionDescriptor>::operator()(const LuaWrapper::LuaFunctionDescriptor& funcDescriptor) const
    {
        return std::hash<std::string>()(funcDescriptor.getFuncName());
    }

    size_t std::hash<LuaWrapper::KeyValuePair>::operator()(const LuaWrapper::KeyValuePair& kvPair) const
    {
        return std::hash<LuaWrapper::LuaWrappable>()(kvPair.getKey());
    }

    size_t std::hash<LuaWrapper::LuaWrappable>::operator()(const LuaWrapper::LuaWrappable& wrappable) const
    {
        switch (wrappable.getType())
        {
            case LUA_TNIL:
                return 0;
            case LUA_TBOOLEAN:
                return 1 + wrappable.getBool();
            case LUA_TLIGHTUSERDATA:
                return reinterpret_cast<size_t>(wrappable.getPtr());
            case LUA_TNUMBER:
                return std::hash<double> {}(wrappable.getDouble());
            case LUA_TSTRING:
                return std::hash<std::string> {}(wrappable.getString());
            case LUA_TTABLE:
                throw LuaWrapper::LuaError("Not Implemented yet");
            case LUA_TFUNCTION:
                throw LuaWrapper::LuaError("Not Implemented yet");
            default:
                throw LuaWrapper::LuaError("Unknown typeID: "s + std::to_string(wrappable.getType()));
        }
    }

}
