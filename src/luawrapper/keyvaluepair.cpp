#include "luaerrors.hpp"
#include "luawrappable.hpp"
#include "luasetoperations.hpp"
#include "keyvaluepair.hpp"

namespace LuaWrapper
{
    KeyValuePair::KeyValuePair(const LuaWrappable& key, const LuaWrappable& value)
    {
        assertNoNullKey(key);
        this->key = new LuaWrappable(key);
        this->value = new LuaWrappable(value);
    }

    KeyValuePair::KeyValuePair(LuaWrappable&& key, LuaWrappable&& value)
    {
        assertNoNullKey(key);
        this->key = new LuaWrappable(std::move(key));
        this->value = new LuaWrappable(std::move(value));

        key = nullptr;
        value = nullptr;
    }

    KeyValuePair::~KeyValuePair()
    {
        if (key)
        {
            delete key;
        }

        if (value)
        {
            delete value;
        }
    }

    void KeyValuePair::setNull()
    {
        key = nullptr;
        value = nullptr;
    }

    void KeyValuePair::assertNoNullKey(const LuaWrappable& key)
    {
        if (key.isNil())
        {
            throw LuaError("attempting to create table entry with nil key");
        }
    }

    const LuaWrappable& KeyValuePair::getKey() const
    {
        return *key;
    }

    void KeyValuePair::setKey(const LuaWrappable& newKey)
    {
        *key = newKey;
    }

    void KeyValuePair::setKey(LuaWrappable&& newKey)
    {
        *key = std::move(newKey);
        newKey = nullptr;
    }

    const LuaWrappable& KeyValuePair::getValue() const
    {
        return *value;
    }

    void KeyValuePair::setValue(const LuaWrappable& newValue)
    {
        *value = newValue;
    }

    void KeyValuePair::setValue(LuaWrappable&& newValue)
    {
        *value = newValue;
        newValue = nullptr;
    }
}
