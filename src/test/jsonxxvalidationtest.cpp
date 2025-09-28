#include <fstream>
#include <iostream>
#include <string>
using namespace std::string_literals;

#include <jsonxx-validation/jsonvalidation.hpp>

#include "jsonxxvalidationtest.hpp"

FetchJsonResult::FetchJsonResult(const std::string& errorMessage) :
    result(errorMessage)
{}

FetchJsonResult::FetchJsonResult(const jsonxx::Object& jsonObject) :
    result(jsonObject)
{}

bool FetchJsonResult::isSuccess() const
{
    return result.index() == 1;
}

const std::string& FetchJsonResult::getErrorMessage() const
{
    return std::get<0>(result);
}

const jsonxx::Object& FetchJsonResult::getJsonObject() const
{
    return std::get<1>(result);
}

FetchJsonResult FetchJsonResult::fetchJsonFromFile(const std::string& fileName)
{
    auto jsonFile = std::ifstream(fileName);

    if (!jsonFile.is_open())
    {
        return "could not open test input "s + fileName;
    }

    jsonxx::Object jsonData;
    const bool success = jsonData.parse(jsonFile);
    if (!success)
    {
        return "could not parse test input "s + fileName;
    }

    return jsonData;
}

// ========================================================================== //

// void TestJsonxxValidation::SetUp()
// {

// }

void printValidationMessages(const JsonValidationResult& result)
{
    if (result.hasMessages())
    {
        std::cerr << "WARNINGS:" << std::endl;
        for (const auto& w : result.getValidationWarnings())
        {
            std::cerr << w << std::endl;
        }
        std::cerr << std::endl;

        std::cerr << "ERRORS:" << std::endl;
        for (const auto& e : result.getValidationErrors())
        {
            std::cerr << e << std::endl;
        }
    }
}

// ========================================================================== //

const std::string TestJsonxxValidation::basePath = "./test/jsonxxValidation/";

TEST_F(TestJsonxxValidation, AllDataTypesSuccess)
{
    using namespace JsonValidation;

    const auto fetchResult = FetchJsonResult::fetchJsonFromFile(basePath + "allTypes.json");
    ASSERT_TRUE(fetchResult.isSuccess()) << fetchResult.getErrorMessage();
    const jsonxx::Object& jsonData = fetchResult.getJsonObject();

    SpecificationSet specs;

#define CAPTUREAS(symbol, emplacement) auto& symbol = const_cast<JsonValidation::Specification&>(*emplacement.first)
    specs.emplace("", TID_Null, false);
    specs.emplace("null", TID_Null);
    specs.emplace("string", TID_String);
    specs.emplace("number", TID_Number);
    specs.emplace("bool", TID_Boolean);
    specs.emplace("list", TID_Array);
    CAPTUREAS(nList, specs.emplace("noInnerValidationList", TID_Array));
    CAPTUREAS(wList, specs.emplace("withInnerValidationList", TID_Array));
    specs.emplace("object", TID_Object);
    CAPTUREAS(nObject, specs.emplace("noInnerValidationObject", TID_Object));
    CAPTUREAS(wObject, specs.emplace("withInnerValidationObject", TID_Object));
    specs.emplace("anyType", TID_NoValidation);

    auto nestedList = Specification("nested", TID_Array);
    nestedList.addChild(Specification("#listItem", TID_String));
    wList.addChild(nestedList);

    wObject.addChild(Specification("inner", TID_Boolean));
#undef CAPTURE_AS

    const auto result = validate(jsonData, specs);

    EXPECT_EQ(result.getValidationWarnings().size(), 0) << "input file produced unexpected warnings";
    EXPECT_EQ(result.getValidationErrors().size(), 0) << "input file produced unexpected errors";

    printValidationMessages(result);
}

TEST_F(TestJsonxxValidation, AllDataTypesErrors)
{
    using namespace JsonValidation;

    const auto fetchResult = FetchJsonResult::fetchJsonFromFile(basePath + "allTypes.json");
    ASSERT_TRUE(fetchResult.isSuccess()) << fetchResult.getErrorMessage();
    const jsonxx::Object& jsonData = fetchResult.getJsonObject();

    SpecificationSet specs;

    specs.emplace("string", TID_Number);
    specs.emplace("missing", TID_Null);

    const auto result = validate(jsonData, specs);

    EXPECT_EQ(result.getValidationWarnings().size(), 11) << "input file produced unexpected number of warnings";
    EXPECT_EQ(result.getValidationErrors().size(), 2) << "input file produced unexpected errors";

    // printValidationMessages(result);
}

TEST_F(TestJsonxxValidation, MutexGroups)
{
    using namespace JsonValidation;

    const auto fetchResult = FetchJsonResult::fetchJsonFromFile(basePath + "mutexGroups.json");
    ASSERT_TRUE(fetchResult.isSuccess()) << fetchResult.getErrorMessage();
    const jsonxx::Object& jsonData = fetchResult.getJsonObject();

    MutexGroup grpA = {"A"};
    MutexGroup grpB = {"B"};
    MutexGroup grpC = {"C", "A"};
    MutexGroup grpD = {"D", "B"};

    AllowedValues allAllowed;

    SpecificationSet specs;

    specs.emplace("A1", TID_Null, false, allAllowed, grpA);
    specs.emplace("A2", TID_Null, false, allAllowed, grpA);
    specs.emplace("B1", TID_Null, false, allAllowed, grpB);
    specs.emplace("B2", TID_Null, false, allAllowed, grpB);
    specs.emplace("C", TID_Null, false, allAllowed);
    specs.emplace("D", TID_Null, false, allAllowed, grpC);
    specs.emplace("E", TID_Null, false, allAllowed, grpD);

    const auto result = validate(jsonData, specs);

    EXPECT_EQ(result.getValidationWarnings().size(), 0) << "input file produced unexpected warnings";
    EXPECT_EQ(result.getValidationErrors().size(), 2) << "input file produced unexpected number of errors";

    // printValidationMessages(result);
}

TEST_F(TestJsonxxValidation, MutexGroupsMandatory)
{
    using namespace JsonValidation;

    const auto fetchResult = FetchJsonResult::fetchJsonFromFile(basePath + "mutexGroups.json");
    ASSERT_TRUE(fetchResult.isSuccess()) << fetchResult.getErrorMessage();
    const jsonxx::Object& jsonData = fetchResult.getJsonObject();

    MutexGroup grpA = {"A"};
    MutexGroup grpB = {"B"};
    MutexGroup grpC = {"C", "A"};
    MutexGroup grpD = {"D", "B"};

    AllowedValues allAllowed;

    SpecificationSet specs;

    specs.emplace("A1", TID_Null, false, allAllowed, grpA);
    specs.emplace("A2", TID_Null, false, allAllowed, grpA);
    specs.emplace("A3", TID_Null, true, allAllowed, grpA);
    specs.emplace("B1", TID_Null, false, allAllowed, grpB);
    specs.emplace("B2", TID_Null, false, allAllowed, grpB);
    specs.emplace("B3", TID_Null, true, allAllowed, grpB);
    specs.emplace("C", TID_Null, false, allAllowed);
    specs.emplace("D", TID_Null, false, allAllowed, grpC);
    specs.emplace("E", TID_Null, false, allAllowed, grpC);
    specs.emplace("F", TID_Null, false, allAllowed, grpD);
    specs.emplace("G", TID_Null, false, allAllowed, grpD);

    const auto result = validate(jsonData, specs);

    EXPECT_EQ(result.getValidationWarnings().size(), 0) << "input file produced unexpected warnings";
    EXPECT_EQ(result.getValidationErrors().size(), 3) << "input file produced unexpected number of errors";

    // printValidationMessages(result);
}

// TODO: TEST_F(TestJsonxxValidation, AllowedValues)
