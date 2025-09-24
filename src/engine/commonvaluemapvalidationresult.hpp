#ifndef COMMONVALUEMAPVALIDATIONRESULT_HPP
#define COMMONVALUEMAPVALIDATIONRESULT_HPP

#include <list>

#include "commonvaluecollection.hpp"
#include "sharedtypes.hpp"

namespace StratStat
{
    class CommonValueMapValidationResult
    {
        public:


        private:
            std::variant<CommonValueCollection, ErrorMessgeList> result;
            ErrorMessgeList warnings;

        public:
            CommonValueMapValidationResult();
            CommonValueMapValidationResult(const CommonValueCollection& result, const ErrorMessgeList& warnings = ErrorMessgeList());
            CommonValueMapValidationResult(const ErrorMessgeList& result);

            std::variant<CommonValueCollection, ErrorMessgeList> getAsVariant() const;

            void addErrorMessage(const std::string& message);
            void addWarning(const std::string& message);

            bool isSuccess() const;
            bool hasWarnings() const;
            const ErrorMessgeList& getWarnings() const;
            const ErrorMessgeList& getErrorMessages() const;
            const CommonValueCollection& getCommonValueCollection() const;

            const std::string collectWarnings() const;
            const std::string collectErrorMessages() const;
    };
}

#endif // COMMONVALUEMAPVALIDATIONRESULT_HPP
