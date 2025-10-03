#ifndef LUATABLECONSTITERATOR_HPP
#define LUATABLECONSTITERATOR_HPP

#include <unordered_map>

namespace LuaWrapper
{
    class IndirectWrappable;
    class LuaWrappable;

    class LuaTableConstIterator
    {
        public:
            using UnderlyingIterator = std::unordered_map<IndirectWrappable, IndirectWrappable>::const_iterator;

            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = std::pair<IndirectWrappable, IndirectWrappable>;
            using pointer           = value_type*;
            using reference         = value_type&;

        private:
            UnderlyingIterator it;

        public:
            LuaTableConstIterator(const UnderlyingIterator& it);

            value_type operator*();
            LuaTableConstIterator& operator++();
            LuaTableConstIterator operator++(int);

            friend bool operator== (const LuaTableConstIterator& lhs, const LuaTableConstIterator& rhs);
            friend bool operator!= (const LuaTableConstIterator& lhs, const LuaTableConstIterator& rhs);
    };
}

#endif // LUATABLECONSTITERATOR_HPP
