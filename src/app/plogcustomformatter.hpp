#ifndef PLOGCUSTOMFORMATTER_HPP
#define PLOGCUSTOMFORMATTER_HPP

#include <plog/Record.h>
#include <plog/Util.h>

class PlogCustomFormatter
{
    public:
        static plog::util::nstring header();
        static plog::util::nstring format(const plog::Record& record);
};

#endif // PLOGCUSTOMFORMATTER_HPP
