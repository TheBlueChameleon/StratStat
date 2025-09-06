#include <iostream>
#include <dlfcn.h>

#include <lua.hpp>
#include <jsonxx.h>

#include "clihandler.hpp"

void doLuaStuff()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "./strategies/hello.lua");
    lua_close(L);
}

int dyLibStuff(const Config& cfg)
{
    void* object{};
    char* error{};
    void* handler{};
    //* load shared library named "your-external-lib.so" file at runtime
    handler = dlopen(cfg.engine.c_str(), RTLD_LAZY);
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
    auto cfg = parseCliArgs(argc, argv);
    doLuaStuff();
    dyLibStuff(cfg);

    return 0;
}
