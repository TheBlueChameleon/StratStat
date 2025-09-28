#ifndef LUAERROR_HPP
#define LUAERROR_HPP

#include <stdexcept>


namespace LuaWrapper
{
    class LuaError : public std::runtime_error
    {
        public:
            using std::runtime_error::runtime_error;
    };
}

#endif // LUAERROR_HPP
