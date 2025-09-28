#ifndef LUAERRORS_H
#define LUAERRORS_H

#include <stdexcept>


namespace LuaWrapper
{
    class LuaError : public std::runtime_error
    {
        public:
            using std::runtime_error::runtime_error;
    };

    // TODO differentiate errors
}

#endif // LUAERRORS_H
