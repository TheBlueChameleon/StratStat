#ifndef JSONVALIDATIONTIONRESULT_H
#define JSONVALIDATIONTIONRESULT_H

#include <string>
#include <vector>


// TODO: introduce class JsonValidationMessages : public std::vector<std::string>?
class JsonValidationResult
{
    private:
        const std::vector<std::string> validationErrors;
        const std::vector<std::string> validationWarnings;

    public:
        JsonValidationResult();
        JsonValidationResult(const std::vector<std::string>& validationErrors, const std::vector<std::string>& warnings);

        bool isValid() const;
        bool hasMessages() const;
        const std::vector<std::string>& getValidationErrors() const;
        std::vector<std::string> getValidationWarnings() const;
};

#endif // JSONVALIDATIONTIONRESULT_H
