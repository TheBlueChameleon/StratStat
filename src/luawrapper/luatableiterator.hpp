#ifndef LUATABLEITERATOR_H
#define LUATABLEITERATOR_H

#include <unordered_map>

namespace LuaWrapper
{
    class LuaWrappable;

    class LuaTableIterator
    {
        public:
            using UnderlyingIterator = std::unordered_map<IndirectWrappable, IndirectWrappable>::iterator;

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = std::pair<LuaWrappable&, LuaWrappable&>;
            using pointer           = value_type*;
            using reference         = value_type&;

        private:
            UnderlyingIterator  it;
            value_type          current;

            void increment();

            LuaWrappable* getKeyPtr() const;
            LuaWrappable* getValuePtr() const;

        public:
            LuaTableIterator(const UnderlyingIterator& it);

            reference operator*();
            pointer operator->();
            LuaTableIterator& operator++();
            LuaTableIterator operator++(int);

            friend bool operator== (const LuaTableIterator& lhs, const LuaTableIterator& rhs);
            friend bool operator!= (const LuaTableIterator& lhs, const LuaTableIterator& rhs);
    };
}


#endif // LUATABLEITERATOR_H
