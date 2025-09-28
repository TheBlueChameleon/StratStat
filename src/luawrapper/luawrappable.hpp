#ifndef LUAWRAPPABLE_HPP
#define LUAWRAPPABLE_HPP

#include <variant>

#include "lua.hpp"
#include "luatable.hpp"
#include "luafunctiondescriptor.hpp"

namespace LuaWrapper
{
    using LuaWrappableVariant = std::variant<nullptr_t, bool, void*, double, std::string, LuaTable, LuaFunctionDescriptor, std::monostate>;

    class LuaWrappable
    {
        private:
            LuaWrappableVariant value;

            void pushToLua(lua_State* L) const;

        public:
            LuaWrappable();
            LuaWrappable(nullptr_t);
            LuaWrappable(bool);
            LuaWrappable(void*);
            LuaWrappable(int);
            LuaWrappable(double);
            LuaWrappable(const std::string&);
            LuaWrappable(const LuaTable&);
            LuaWrappable(const LuaFunctionDescriptor&);

            int getType() const;

            void setValue(const LuaWrappableVariant& newV);
            LuaWrappableVariant getValue() const;

            bool getBool() const;
            void* getPtr() const;
            int getInt() const;
            double getDouble() const;
            const std::string& getString() const;
            const LuaTable& getTable() const;
            const LuaFunctionDescriptor& getFunctionDescriptor() const;

            friend class ParameterStack;
    };
}

#endif // LUAWRAPPABLE_HPP
