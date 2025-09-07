#include <iostream>
#include <dlfcn.h>

#include <lua.hpp>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include "cliparser.hpp"


void configureLogger(const Config& cfg)
{
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;

    const auto severity = static_cast<plog::Severity>(cfg.getLogLevel());

    if (cfg.getLogFile().has_value())
    {
        static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(cfg.getLogFile().value().c_str());

        plog::init(severity, &consoleAppender)
        .addAppender(&fileAppender);
    }
    else
    {
        plog::init(severity, &consoleAppender);
    }
}

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
    handler = dlopen(cfg.getEngine().c_str(), RTLD_LAZY);
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

int main(const int argc, const char* argv[])
{
    auto parser = CliParser();
    auto cfg = parser.run(argc, argv);

    configureLogger(cfg);

    // doLuaStuff();
    // dyLibStuff(cfg);

    PLOG_VERBOSE << "verbose 6";
    PLOG_DEBUG << "debug 5";
    PLOG_INFO << "info 4";
    PLOG_WARNING << "warning 3";
    PLOG_ERROR << "error 2";
    PLOG_FATAL << "fatal 1";

    return 0;
}
