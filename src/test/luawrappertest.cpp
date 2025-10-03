#include <iostream>
#include <string>
using namespace std::string_literals;

#include <app/errors.hpp>

#include <luawrapper/luawrapper.hpp>

#include "luawrappertest.hpp"
using namespace LuaWrapper;

const std::string LuaWrapperTest::basePath = "./test/LuaWrapper/";

TEST_F(LuaWrapperTest, InitUnsuccessful)
{
    ASSERT_THROW(LuaState(basePath + "doesNotExist"), CriticalAbort);

    ASSERT_THROW(LuaState(basePath + "malformed.lua"), CriticalAbort);

}

TEST_F(LuaWrapperTest, TableAccess)
{
    constexpr auto TRUE = "TRUE";
    LuaTable t;

    ASSERT_THROW(t.setEntry(nullptr, nullptr), LuaError);

    t.setEntry(0, nullptr);         // move semantics
    EXPECT_TRUE(t.hasKey(0));
    EXPECT_FALSE(t.hasKey(1));

    LuaWrappable key = true;        // copy semantics
    LuaWrappable value = TRUE;
    t.setEntry(key, value);

    EXPECT_EQ(t.size(), 2);

    std::unordered_set<LuaWrappable> expectedKeys = {0, true};
    std::unordered_set<LuaWrappable> actualKeys = t.getKeySet();
    EXPECT_EQ(actualKeys, expectedKeys);

    std::unordered_multiset<LuaWrappable> expectedValues = {nullptr, TRUE};
    std::unordered_multiset<LuaWrappable> actualValues = t.getValues();
    EXPECT_EQ(actualValues, expectedValues);

    // overwrite table value
    t.setEntry(0, 0);
    actualKeys = t.getKeySet();
    actualValues = t.getValues();
    EXPECT_EQ(t.size(), 2);
    EXPECT_EQ(actualKeys, expectedKeys);
    EXPECT_NE(actualValues, expectedValues);

    // iterability
    std::unordered_map<LuaWrappable, LuaWrappable> expected =
    {
        {0, 0},
        {true, TRUE}
    };
    for (const auto& [k, v]  : t)
    {
        EXPECT_EQ(expected.at(k.get()), v);
    }

}

TEST_F(LuaWrapperTest, CommunicationNumbers)
{
    const std::string name = "identity";
    auto fd = LuaFunctionDescriptor(name, {LUA_TNUMBER}, {LUA_TNUMBER});
    auto ps1 = ParameterStack({1});
    auto ps2 = ParameterStack({1.5});

    LuaState state(basePath + "identity.lua");

    state.registerLuaFunction(fd);
    const auto re1 = state.invoke(name, ps1);
    const auto re2 = state.invoke(name, ps2);

    ASSERT_EQ(re1.size(), 1);
    EXPECT_EQ(re1[0].getInt(), 1);
    ASSERT_EQ(re2.size(), 1);
    EXPECT_EQ(re2[0].getDouble(), 1.5);
}

TEST_F(LuaWrapperTest, CommunicationStrings)
{
    const std::string name = "identity";
    const std::string arg = "foo thy bar";
    auto fd = LuaFunctionDescriptor(name, {LUA_TSTRING}, {LUA_TSTRING});
    auto ps = ParameterStack({arg});
    LuaState state(basePath + "identity.lua");

    state.registerLuaFunction(fd);
    const auto re = state.invoke(name, ps);

    ASSERT_EQ(re.size(), 1);
    EXPECT_EQ(re[0].getString(), arg);
}

TEST_F(LuaWrapperTest, CommunicationTables)
{
    const std::string name = "identity";
    const LuaTable arg;
    auto fd = LuaFunctionDescriptor(name, {LUA_TTABLE}, {LUA_TTABLE});
    auto ps = ParameterStack({arg});
    LuaState state(basePath + "identity.lua");

    state.registerLuaFunction(fd);
    const auto re = state.invoke(name, ps);

    ASSERT_EQ(re.size(), 1);
    EXPECT_EQ(re[0].getTable(), arg);
}

TEST_F(LuaWrapperTest, CommunicationsMultivariate)
{
    const std::string name = "sumAndDifference";
    auto fd = LuaFunctionDescriptor(name, {LUA_TNUMBER, LUA_TNUMBER}, {LUA_TNUMBER, LUA_TNUMBER});
    auto ps = ParameterStack({1, 2});

    LuaState state(basePath + "parameters.lua");
    state.registerLuaFunction(fd);

    const auto re = state.invoke(name, ps);

    ASSERT_EQ(re.size(), 2);
    EXPECT_EQ(re[0].getDouble(), +3);
    EXPECT_EQ(re[1].getDouble(), -1);
}

TEST_F(LuaWrapperTest, CommunicationsInsuccessful)
{
    const std::string name = "identity";
    auto fd = LuaWrapper::LuaFunctionDescriptor("doesNotExist", {LUA_TNUMBER}, {LUA_TNUMBER});
    auto ps = ParameterStack({"foo"s});

    LuaState state(basePath + "identity.lua");

    // does not exist
    ASSERT_THROW(state.registerLuaFunction(fd), LuaError);

    // not (yet) known
    ASSERT_THROW(state.invoke(name, ps), LuaError);

    fd = LuaFunctionDescriptor(name, {LUA_TNUMBER}, {LUA_TNUMBER});
    state.registerLuaFunction(fd);
    // type mismatch
    ASSERT_THROW(state.invoke(name, ps), LuaError);

}
