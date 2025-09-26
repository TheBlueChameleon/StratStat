#ifndef TESTJSONXXVALIDATION_HPP
#define TESTJSONXXVALIDATION_HPP

#include <gtest/gtest.h>

#include <jsonxx.h>

#include "jsonxx-validation/jsonvalidation.hpp"

class FetchJsonResult
{
    private:
        std::variant<std::string, jsonxx::Object> result;

    public:
        FetchJsonResult(const std::string& errorMessage);
        FetchJsonResult(const jsonxx::Object& jsonObject);

        bool isSuccess() const;
        const std::string& getErrorMessage() const;
        const jsonxx::Object& getJsonObject() const;

        static FetchJsonResult fetchJsonFromFile(const std::string& fileName);
};

class TestJsonxxValidation : public testing::Test
{
    protected:
        // void SetUp() override;

        const static std::string basePath;


};

#endif // TESTJSONXXVALIDATION_HPP
