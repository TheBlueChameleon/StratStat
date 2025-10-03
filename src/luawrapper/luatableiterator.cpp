#include <iostream>

#include "luatable.hpp"
#include "luatableiterator.hpp"
#include "luawrappable.hpp"

namespace LuaWrapper
{
    LuaTableIterator::LuaTableIterator(const UnderlyingIterator& it) :
        it(it),
        current({*getKeyPtr(), *getValuePtr()})
    {
        std::cout << "constructed iterator" << std::endl;
        std::cout << "  with key = " << getKeyPtr()   << ": " << getKeyPtr()->getRepr() << std::endl;
        std::cout << "  with val = " << getValuePtr() << ": " << getValuePtr()->getRepr() << std::endl;
    }

    LuaWrappable* LuaTableIterator::getKeyPtr() const
    {
        return it->first.expose();
    }

    LuaWrappable* LuaTableIterator::getValuePtr() const
    {
        return it->second.expose();
    }

    LuaTableIterator::reference LuaTableIterator::operator*()
    {
        std::cout << "about to deref" << std::endl;
        return current;
    }

    LuaTableIterator::pointer LuaTableIterator::operator->()
    {
        std::cout << "about to deref via ptr" << std::endl;
        return &current;
    }
    void LuaTableIterator::increment()
    {
        std::cout << "about to increment" << std::endl;
        ++it;
        current = std::make_pair(*getKeyPtr(), *getValuePtr());
        std::cout << "inc'd" << std::endl;
    }

    LuaTableIterator& LuaTableIterator::operator++()
    {
        std::cout << "about to ++" << std::endl;
        increment();;
        return *this;
    }

    LuaTableIterator LuaTableIterator::operator++(int)
    {
        std::cout << "about to ++(int)" << std::endl;
        const UnderlyingIterator tmpIt = it;
        increment();
        return LuaTableIterator(tmpIt);
    }

    bool operator== (const LuaTableIterator& lhs, const LuaTableIterator& rhs)
    {
        std::cout << "about to comp eq" << std::endl;
        return lhs.it == rhs.it;
    }
    bool operator!= (const LuaTableIterator& lhs, const LuaTableIterator& rhs)
    {
        std::cout << "about to comp ne" << std::endl;
        return lhs.it != rhs.it;
    }
}
