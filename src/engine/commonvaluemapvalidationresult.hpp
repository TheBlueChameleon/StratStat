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
            using ErrorMessgeList = std::list<std::string>;

        private:
            std::variant<CommonValueCollection, ErrorMessgeList> result;

        public:
            CommonValueMapValidationResult();
            CommonValueMapValidationResult(const CommonValueCollection& result);
            CommonValueMapValidationResult(const ErrorMessgeList& result);

            std::variant<CommonValueCollection, ErrorMessgeList> getAsVariant() const;

            void addErrorMessage(const std::string& message);

            bool isSuccess() const;
            const ErrorMessgeList& getErrorMessages() const;
            const CommonValueCollection& getCommonValueCollection() const;

            const std::string collectErrorMessages() const;
    };
}

#endif // COMMONVALUEMAPVALIDATIONRESULT_HPP
