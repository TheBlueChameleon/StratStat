#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <stdexcept>

class CriticalAbort : public std::runtime_error
{
    public:
        using std::runtime_error::runtime_error;

        CriticalAbort() :
            std::runtime_error("Critical abort. See the logs for possible reasons.")
        {};
};

#endif // ERRORS_HPP
