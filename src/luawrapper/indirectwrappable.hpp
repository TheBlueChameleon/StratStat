#ifndef INDIRECTWRAPPABLE_HPP
#define INDIRECTWRAPPABLE_HPP


namespace LuaWrapper
{
    class LuaWrappable;

    class IndirectWrappable
    {
        private:
            LuaWrappable* ptr = nullptr;

        public:
            IndirectWrappable(const IndirectWrappable& other);
            IndirectWrappable(IndirectWrappable&& other);
            IndirectWrappable(const LuaWrappable& other);
            IndirectWrappable(LuaWrappable&& other);
            ~IndirectWrappable();

            IndirectWrappable& operator=(const IndirectWrappable& other) = default;
            IndirectWrappable& operator=(IndirectWrappable&& other) = default;

            bool holdsValue() const;

            const LuaWrappable& get() const;
            void set(const LuaWrappable& newEntity);
            void set(LuaWrappable&& newEntity);

            friend class LuaTable;
    };
}

#endif // INDIRECTWRAPPABLE_HPP
