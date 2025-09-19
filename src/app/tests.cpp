#include <iostream>
#include <fstream>

#include <jsonxx.h>

#include "../json-schema/jsonvalidation.hpp"
#include "tests.hpp"

using namespace JsonValidation;

void testJsonValidation()
{
    const auto jsonFileName = "./test/jsonTestCase.json";
    auto jsonFile = std::ifstream(jsonFileName);

    jsonxx::Object jsonData;
    const bool success = jsonData.parse(jsonFile);
    if (!success)
    {
        std::cerr << "The file " << jsonFileName << " is not a valid JSON file." << std::endl;
        std::exit(-1);
    }

    SpecificationSet specs;
    MutexGroup grpA = {"A"};
    MutexGroup grpB = {"B"};
    MutexGroup grpAB = {"A", "B"};

    AllowedValues allAllowed;

#define CAPTUREAS(symbol, emplacement) auto& symbol = const_cast<JsonValidation::Specification&>(*emplacement.first)

    specs.emplace("", TID_Null, false);
    specs.emplace("null", TID_Null);
    specs.emplace("string", TID_String);
    specs.emplace("number", TID_Number);
    specs.emplace("bool", TID_Boolean);
    CAPTUREAS(list, specs.emplace("list", TID_Array));
    CAPTUREAS(nested, specs.emplace("nested", TID_Array));
    CAPTUREAS(object, specs.emplace("object", TID_Object));
    CAPTUREAS(mutex, specs.emplace("mutexGroups", TID_Object));
    specs.emplace("wrongType", TID_Null);
    specs.emplace("anyType", TID_NoValidation);

    object.addChild(Specification("inner", TID_Boolean));

    mutex.addChild(Specification("A", TID_Null, true, allAllowed, grpA));
    mutex.addChild(Specification("B", TID_Null, true, allAllowed, grpB));
    mutex.addChild(Specification("AB", TID_Null, true, allAllowed, grpAB));

    list.addChild(Specification("", TID_Number));

    auto stringList = Specification("", TID_Array);
    stringList.addChild(Specification("", TID_String));
    nested.addChild(stringList);
#undef CAPTURE_AS

    const auto result = validate(jsonData, specs);

    std::cout << "WARNINGS:" << std::endl;
    for (const auto& w : result.getValidationWarnings())
    {
        std::cout << w << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ERRORS:" << std::endl;
    for (const auto& e : result.getValidationErrors())
    {
        std::cout << e << std::endl;
    }
}

void testTeamDefJson()
{

}
