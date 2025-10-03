#ifndef LUAWRAPPABLE_HPP
#define LUAWRAPPABLE_HPP

#include <variant>

#include "luacapi.hpp"
#include "luasetoperations.hpp"
#include "luatable.hpp"
#include "luafunctiondescriptor.hpp"

namespace LuaWrapper
{
    class LuaWrappableVariant : public std::variant<nullptr_t, bool, void*, double, std::string, LuaTable, LuaFunctionDescriptor, std::monostate>
    {
        public:
            using std::variant<nullptr_t, bool, void*, double, std::string, LuaTable, LuaFunctionDescriptor, std::monostate>::variant;

    };

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
            LuaWrappable(const char* const);
            LuaWrappable(const std::string&);
            LuaWrappable(std::string&&);
            LuaWrappable(const LuaTable&);
            LuaWrappable(LuaTable&&);
            LuaWrappable(const LuaFunctionDescriptor&);

            bool isNil() const;
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

            const std::string getRepr() const;

            friend class ParameterStack;
    };
}

#endif // LUAWRAPPABLE_HPP
