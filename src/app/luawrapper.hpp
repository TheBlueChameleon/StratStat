#ifndef LUAWRAPPER_HPP
#define LUAWRAPPER_HPP

#include <filesystem>

#include <lua.hpp>

class LuaWrapper
{
    private:
        lua_State* L = nullptr;

    public:
        LuaWrapper(const std::filesystem::path& scriptFile);
        ~LuaWrapper();
};

#endif // LUAWRAPPER_HPP
