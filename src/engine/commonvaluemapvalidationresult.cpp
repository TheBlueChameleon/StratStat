#include <string>
#include <sstream>

#include "commonvaluemapvalidationresult.hpp"

namespace StratStat
{

    CommonValueMapValidationResult::CommonValueMapValidationResult() :
        result(std::list<std::string>())
    {}

    CommonValueMapValidationResult::CommonValueMapValidationResult(const CommonValueCollection& result) :
        result(result)
    {}

    CommonValueMapValidationResult::CommonValueMapValidationResult(const ErrorMessgeList& result) :
        result(result)
    {}

    std::variant<CommonValueCollection, CommonValueMapValidationResult::ErrorMessgeList> CommonValueMapValidationResult::getAsVariant() const
    {
        return result;
    }

    void CommonValueMapValidationResult::addErrorMessage(const std::string& message)
    {
        std::get<ErrorMessgeList>(result).push_back(message);
    }

    bool CommonValueMapValidationResult::isSuccess() const
    {
        return result.index() == 0;
    }

    const CommonValueMapValidationResult::ErrorMessgeList& CommonValueMapValidationResult::getErrorMessages() const
    {
        return std::get<ErrorMessgeList>(result);
    }

    const CommonValueCollection& CommonValueMapValidationResult::getCommonValueCollection() const
    {
        return std::get<CommonValueCollection>(result);
    }

    const std::string CommonValueMapValidationResult::collectErrorMessages() const
    {
        std::stringstream ss;

        for (const auto& msg: getErrorMessages())
        {
            ss << msg << std::endl;
        }

        return ss.str();
    }
}
