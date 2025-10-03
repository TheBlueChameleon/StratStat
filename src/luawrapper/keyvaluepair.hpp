#ifndef KEYVALUEPAIR_HPP
#define KEYVALUEPAIR_HPP

#include <unordered_set>

namespace LuaWrapper
{
    class LuaWrappable;

    class KeyValuePair
    {
        private:
            // using pointers here to avoid circular dependency issues
            LuaWrappable* key;
            LuaWrappable* value;

            void setNull();
            void assertNoNullKey(const LuaWrapper::LuaWrappable& key);

        public:
            KeyValuePair(const KeyValuePair& other);
            KeyValuePair(KeyValuePair&& other);
            KeyValuePair(const LuaWrappable& key, const LuaWrappable& value);
            KeyValuePair(LuaWrappable&& key, LuaWrappable&& value);
            ~KeyValuePair();

            const LuaWrappable& getKey() const;
            void setKey(const LuaWrappable& newKey);
            void setKey(LuaWrappable&& newKey);

            const LuaWrappable& getValue() const;
            void setValue(const LuaWrappable& newValue);
            void setValue(LuaWrappable&& newValue);

            friend class LuaTable;
    };
}

#endif // KEYVALUEPAIR_HPP
