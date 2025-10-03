#include <format>
#include <string>
using namespace std::string_literals;

#include "luaerrors.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaWrappable::LuaWrappable(nullptr_t) :
        value(nullptr)
    {}

    LuaWrappable::LuaWrappable(bool b) :
        value(b)
    {}

    LuaWrappable::LuaWrappable(void* ptr) :
        value(ptr)
    {}

    LuaWrappable::LuaWrappable(int i) :
        value(static_cast<double>(i))
    {}

    LuaWrappable::LuaWrappable(double d) :
        value(d)
    {}

    LuaWrappable::LuaWrappable(const char* const s) :
        value(std::string(s))
    {}

    LuaWrappable::LuaWrappable(const std::string& s) :
        value(s)
    {}

    LuaWrappable::LuaWrappable(std::string&& s) :
        value(std::move(s))
    {}

    LuaWrappable::LuaWrappable(const LuaTable& t) :
        value(t)
    {}

    LuaWrappable::LuaWrappable(LuaTable&& t) :
        value(std::move(t))
    {}

    LuaWrappable::LuaWrappable(const LuaFunctionDescriptor& f) :
        value(f)
    {}

    LuaWrappable::LuaWrappable(const LuaWrappable& other) :
        value(other.value)
    {}

    LuaWrappable::LuaWrappable(LuaWrappable&& other) :
        value(std::move(other.value))
    {}

    bool LuaWrappable::isNil() const
    {
        return getType() == LUA_TNIL;
    }

    bool LuaWrappable::isBool() const
    {
        return getType() == LUA_TBOOLEAN;
    }

    bool LuaWrappable::isPtr() const
    {
        return getType() == LUA_TLIGHTUSERDATA;
    }

    bool LuaWrappable::isNumber() const
    {
        return getType() == LUA_TNUMBER;
    }

    bool LuaWrappable::isString() const
    {
        return getType() == LUA_TSTRING;
    }

    bool LuaWrappable::isTable() const
    {
        return getType() == LUA_TTABLE;
    }

    bool LuaWrappable::isFunction() const
    {
        return getType() == LUA_TFUNCTION;
    }

    bool LuaWrappable::isError() const
    {
        return getType() == LUA_NUMTYPES;
    }

    void LuaWrappable::pushToLua(lua_State* L) const
    {
        switch (getType())
        {
            case LUA_TNIL:
                lua_pushnil(L);
                break;
            case LUA_TBOOLEAN:
                lua_pushboolean(L, getBool());
                break;
            case LUA_TLIGHTUSERDATA:
                lua_pushlightuserdata(L, getPtr());
                break;
            case LUA_TNUMBER:
                lua_pushnumber(L, getDouble());
                break;
            case LUA_TSTRING:
                lua_pushstring(L, getString().c_str());
                break;
            case LUA_TTABLE:
                getTable().pushToLua(L);
                break;
            case LUA_TFUNCTION:
                throw LuaError("Not Implemented yet");
            default:
                throw LuaError("Unknown typeID: "s + std::to_string(getType()));
        }
    }

    int LuaWrappable::getType() const
    {
        return value.index();
    }

    void LuaWrappable::setValue(const LuaWrappableVariant& newV)
    {
        value = newV;
    }

    LuaWrappableVariant LuaWrappable::getValue() const
    {
        return value;
    }

    bool LuaWrappable::getBool() const
    {
        return std::get<bool>(value);
    }

    void* LuaWrappable::getPtr() const
    {
        return std::get<void*>(value);
    }

    int LuaWrappable::getInt() const
    {
        return std::get<double>(value);
    }

    double LuaWrappable::getDouble() const
    {
        return std::get<double>(value);
    }

    const std::string& LuaWrappable::getString() const
    {
        return std::get<std::string>(value);
    }

    const LuaTable& LuaWrappable::getTable() const
    {
        return std::get<LuaTable>(value);
    }

    const LuaFunctionDescriptor& LuaWrappable::getFunctionDescriptor() const
    {
        return std::get<LuaFunctionDescriptor>(value);
    }

    const std::string LuaWrappable::getRepr() const
    {
        switch (getType())
        {
            case LUA_TNIL:
                return "nil";
            case LUA_TBOOLEAN:
                return (getBool() ? "true" : "false");
            case LUA_TLIGHTUSERDATA:
                return "0x"s + std::format("{:x}", reinterpret_cast<size_t>(getPtr()));;
            case LUA_TNUMBER:
                return std::to_string(getDouble());
            case LUA_TSTRING:
                return getString();
            case LUA_TTABLE:
                throw LuaError("Not Implemented yet");
            case LUA_TFUNCTION:
                throw LuaError("Not Implemented yet");
            default:
                throw LuaError("Unknown typeID: "s + std::to_string(getType()));
        }
    }
}
