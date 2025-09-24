#include <string>
#include <sstream>

#include "commonvaluemapvalidationresult.hpp"

namespace StratStat
{
    CommonValueMapValidationResult::CommonValueMapValidationResult() :
        result(ErrorMessgeList())
    {}

    CommonValueMapValidationResult::CommonValueMapValidationResult(
        const CommonValueCollection& result,
        const ErrorMessgeList& warnings
    ) :
        result(result),
        warnings(warnings)
    {}

    CommonValueMapValidationResult::CommonValueMapValidationResult(const ErrorMessgeList& result) :
        result(result)
    {}

    std::variant<CommonValueCollection, ErrorMessgeList> CommonValueMapValidationResult::getAsVariant() const
    {
        return result;
    }

    void CommonValueMapValidationResult::addErrorMessage(const std::string& message)
    {
        std::get<ErrorMessgeList>(result).push_back(message);
    }

    void CommonValueMapValidationResult::addWarning(const std::string& message)
    {
        warnings.push_back(message);
    }

    bool CommonValueMapValidationResult::isSuccess() const
    {
        return result.index() == 0;
    }

    bool CommonValueMapValidationResult::hasWarnings() const
    {
        return !warnings.empty();
    }

    const ErrorMessgeList& CommonValueMapValidationResult::getWarnings() const
    {
        return warnings;
    }

    const ErrorMessgeList& CommonValueMapValidationResult::getErrorMessages() const
    {
        return std::get<ErrorMessgeList>(result);
    }

    const CommonValueCollection& CommonValueMapValidationResult::getCommonValueCollection() const
    {
        return std::get<CommonValueCollection>(result);
    }

    const std::string CommonValueMapValidationResult::collectWarnings() const
    {
        std::stringstream ss;

        for (const auto& msg: getWarnings())
        {
            ss << msg << std::endl;
        }

        return ss.str();
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
