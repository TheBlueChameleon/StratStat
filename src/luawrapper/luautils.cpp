#include <string>
using namespace std::string_literals;

#include "luacapi.hpp"
#include "luautils.hpp"
#include "luaerrors.hpp"

namespace LuaWrapper
{
    std::string getTypeName(const int typeID)
    {
        switch (typeID)
        {
            case LUA_TNIL:
                return "null";
            case LUA_TBOOLEAN:
                return "boolean";
            case LUA_TLIGHTUSERDATA:
                return "light userdata";
            case LUA_TNUMBER:
                return "number";
            case LUA_TSTRING:
                return "string";
            case LUA_TTABLE:
                return "table";
            case LUA_TFUNCTION:
                return "function";
            case LUA_TUSERDATA:
                return "userdata";
            case LUA_TTHREAD:
                return "thread";
            default:
                return "unknown typeID: "s + std::to_string(typeID);
        }
    }
}
