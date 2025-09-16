#include "jsonvalidationtionresult.hpp"

JsonValidationResult::JsonValidationResult()
{}

JsonValidationResult::JsonValidationResult(
    const std::vector<std::string>& validationErrors,
    const std::vector<std::string>& validationWarnings
):
    validationErrors(validationErrors),
    validationWarnings(validationWarnings)
{}

bool JsonValidationResult::isValid() const
{
    return validationErrors.empty();
}

bool JsonValidationResult::hasMessages() const
{
    return !(validationErrors.empty() && validationWarnings.empty());
}

const std::vector<std::string>& JsonValidationResult::getValidationErrors() const
{
    return validationErrors;
}

std::vector<std::string> JsonValidationResult::getValidationWarnings() const
{
    return validationWarnings;
}
