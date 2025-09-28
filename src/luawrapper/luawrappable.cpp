#include "luaerrors.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaWrappable::LuaWrappable() :
        value(std::monostate())
    {}

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

    LuaWrappable::LuaWrappable(const std::string& s) :
        value(s)
    {}

    LuaWrappable::LuaWrappable(const LuaTable& t) :
        value(t)
    {}

    LuaWrappable::LuaWrappable(const LuaFunctionDescriptor& f) :
        value(f)
    {}

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
}
