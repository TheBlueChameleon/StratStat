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

    std::unordered_set<Node> specs;
    std::unordered_set<std::string> grpA = {"A"};
    std::unordered_set<std::string> grpB = {"B"};
    std::unordered_set<std::string> grpAB = {"A", "B"};

#define CAPTUREAS(symbol, emplacement) auto& symbol = const_cast<JsonValidation::Node&>(*emplacement.first)

    specs.emplace("", Null, false);
    specs.emplace("null", Null);
    specs.emplace("string", String);
    specs.emplace("number", Number);
    specs.emplace("bool", Bool);
    CAPTUREAS(list, specs.emplace("list", Array));
    CAPTUREAS(nested, specs.emplace("nested", Array));
    CAPTUREAS(object, specs.emplace("object", Object));
    CAPTUREAS(mutex, specs.emplace("mutexGroups", Object));
    specs.emplace("wrongType", Null);
    specs.emplace("anyType", NoValidation);

    object.addChild(Node("inner", Bool));

    mutex.addChild(Node("A", Null, true, grpA));
    mutex.addChild(Node("B", Null, true, grpB));
    mutex.addChild(Node("AB", Null, true, grpAB));

    list.addChild(Node("", Number));

    auto stringList = Node("", Array);
    stringList.addChild(Node("", String));
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
