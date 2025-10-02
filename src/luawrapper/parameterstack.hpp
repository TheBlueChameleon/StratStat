#ifndef PARAMETERSTACK_HPP
#define PARAMETERSTACK_HPP

#include <deque>

#include "luacapi.hpp"

namespace LuaWrapper
{
    class LuaWrappable;

    class ParameterStack : public std::deque<LuaWrappable>
    {
        private:
            void pushToLua(lua_State* L) const;

        public:
            using std::deque<LuaWrappable>::deque;

            friend class LuaState;
    };
}

#endif // PARAMETERSTACK_HPP
