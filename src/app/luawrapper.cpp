#include <string>
using namespace std::string_literals;

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

#include "errors.hpp"
#include "luawrapper.hpp"

LuaWrapper::LuaWrapper(const std::filesystem::path& scriptFile)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadfile(L, scriptFile.c_str()))
    {
        throw CriticalAbort("Could not load script '"s + scriptFile.c_str() + "'.");
    }

    if (lua_pcall(L, 0, 0, 0))
    {
        throw CriticalAbort("Could not initialize script '"s + scriptFile.c_str() + "'.");
    }
}

LuaWrapper::~LuaWrapper()
{
    if (L)
    {
        lua_close(L);
    }
    L = nullptr;
}
