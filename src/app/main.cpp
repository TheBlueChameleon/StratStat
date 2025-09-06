#include <iostream>
#include <dlfcn.h>

#include <argparse/argparse.hpp>
#include <lua.hpp>
#include <jsonxx.h>

#include "config.hpp"

Config parseCliArgs(int argc, char* argv[])
{
    argparse::ArgumentParser stratStat("StratStat");

    stratStat
    .add_argument("engine")
    .help("The file path of a BattleEngine library file (DLL/SO file)");

    stratStat
    .add_argument("userTeam")
    .help("The file path of a user team definition file (JSON file)");

    stratStat
    .add_argument("userStrategy")
    .help("The file path of a user strategy definition file (LUA file)");

    stratStat
    .add_argument("enemyTeam")
    .help("The file path of a user team definition file (JSON file)");

    stratStat
    .add_argument("enemyStrategy")
    .help("The file path of a user strategy definition file (LUA file)");

    stratStat
    .add_argument("--repetitions", "-r")
    .help("The number of battles the engine attempts with the specified teams (default: 10)")
    .default_value(10);

    try
    {
        auto cfg = Config();
        stratStat.parse_args(argc, argv);
        return cfg;
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << stratStat;
        std::exit(-1);
    }
}

void doLuaStuff()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "../scripts/hello.lua");
    lua_close(L);
}

int dyLibStuff()
{
    void* object{};
    char* error{};
    void* handler{};
    //* load shared library named "your-external-lib.so" file at runtime
    handler = dlopen("./libStratStat-Engine.so", RTLD_LAZY);
    if (!handler)
    {
        std::cerr << dlerror() << std::endl;
        return -1;
    }

    dlerror(); // clear the error

    //* locate the "symbol"/function inside the library
    auto* func = (long*)dlsym(handler, "dummy");
    if ((error = dlerror()) != nullptr && !func)
    {
        std::cerr << error << std::endl;
        return -1;
    }

    //* cast the variable "object" into the appropriate function pointer type
    auto funcFull = reinterpret_cast<int(*)()>(func);
    std::cout << "LIB ANSWERS: " << funcFull() << std::endl;

    //* close the shared library
    dlclose(handler);

    return 0;
}

int main(int argc, char* argv[])
{
    //doLuaStuff();
    parseCliArgs(argc, argv);
    dyLibStuff();

    return 0;
}
