#ifndef PARAMETERSTACK_HPP
#define PARAMETERSTACK_HPP

#include <deque>

#include "lua.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    class ParameterStack : public std::deque<LuaWrappable>
    {
        private:
            void pushToLua(lua_State* L) const;

        public:
            using std::deque<LuaWrappable>::deque;

            friend class State;
    };
}

#endif // PARAMETERSTACK_HPP
