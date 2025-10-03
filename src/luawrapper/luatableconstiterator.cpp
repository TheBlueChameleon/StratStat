#include "luatable.hpp"
#include "luatableconstiterator.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaTableConstIterator::LuaTableConstIterator(const UnderlyingIterator& it) :
        it(it)
    {}

    LuaTableConstIterator::value_type LuaTableConstIterator::operator*()
    {
        return *it;
    }

    LuaTableConstIterator& LuaTableConstIterator::operator++()
    {
        ++it;
        return *this;
    }

    LuaTableConstIterator LuaTableConstIterator::operator++(int)
    {
        const UnderlyingIterator tmpIt = it++;
        return LuaTableConstIterator(tmpIt);
    }

    bool operator== (const LuaTableConstIterator& lhs, const LuaTableConstIterator& rhs)
    {
        return lhs.it == rhs.it;
    }
    bool operator!= (const LuaTableConstIterator& lhs, const LuaTableConstIterator& rhs)
    {
        return lhs.it != rhs.it;
    }
}
