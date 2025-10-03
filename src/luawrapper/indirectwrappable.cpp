#include "indirectwrappable.hpp"
#include "luaerrors.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    class LuaWrappable;
    IndirectWrappable::IndirectWrappable(const IndirectWrappable& other) :
        ptr(new LuaWrappable(other.get()))
    {}

    IndirectWrappable::IndirectWrappable(IndirectWrappable&& other) :
        ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    IndirectWrappable::IndirectWrappable(const LuaWrappable& other) :
        ptr(new LuaWrappable(other))
    {}

    IndirectWrappable::IndirectWrappable(LuaWrappable&& other) :
        ptr(new LuaWrappable(std::move(other)))
    {}

    IndirectWrappable::~IndirectWrappable()
    {
        if (ptr)
        {
            delete (ptr);
            ptr = nullptr;
        }
    }

    bool IndirectWrappable::holdsValue() const
    {
        return ptr != nullptr;
    }

    const LuaWrappable& IndirectWrappable::get() const
    {
        if (ptr == nullptr)
        {
            throw LuaError("IndirectWrappable is in moved-from state");
        }
        return *ptr;
    }

    void IndirectWrappable::set(const LuaWrappable& newEntity)
    {
        if (ptr == nullptr)
        {
            *this = IndirectWrappable(newEntity);
        }
        else
        {
            *ptr = newEntity;
        }
    }

    void IndirectWrappable::set(LuaWrappable&& newEntity)
    {
        if (ptr == nullptr)
        {
            *this = IndirectWrappable(std::move(newEntity));
        }
        else
        {
            *ptr = std::move(newEntity);
        }
    }

    LuaWrappable& IndirectWrappable::operator*()
    {
        return *ptr;
    }

    LuaWrappable* IndirectWrappable::operator->()
    {
        return ptr;
    }

    const LuaWrappable* IndirectWrappable::operator->() const
    {
        return ptr;
    }

    const LuaWrappable* IndirectWrappable::expose() const
    {
        return ptr;
    }

    LuaWrappable* IndirectWrappable::exposeMutable()
    {
        return ptr;
    }
}
