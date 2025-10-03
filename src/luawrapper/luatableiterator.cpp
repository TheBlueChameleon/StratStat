#include "luatable.hpp"
#include "luatableiterator.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaTableIterator::LuaTableIterator(const UnderlyingIterator& it) :
        it(it)
    {}

    LuaTableIterator::value_type LuaTableIterator::operator*()
    {
        return *it;
    }

    LuaTableIterator& LuaTableIterator::operator++()
    {
        ++it;
        return *this;
    }

    LuaTableIterator LuaTableIterator::operator++(int)
    {
        const UnderlyingIterator tmpIt = it++;
        return LuaTableIterator(tmpIt);
    }

    bool operator== (const LuaTableIterator& lhs, const LuaTableIterator& rhs)
    {
        return lhs.it == rhs.it;
    }
    bool operator!= (const LuaTableIterator& lhs, const LuaTableIterator& rhs)
    {
        return lhs.it != rhs.it;
    }
}
