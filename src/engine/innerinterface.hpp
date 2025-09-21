#ifndef INNERINTERFACE_HPP
#define INNERINTERFACE_HPP

#include <optional>

#include <jsonxx.h>

#include "sharedtypes.hpp"

namespace StratStat
{
    StratStat::CommonValueMapValidationResult validatePkmnDef(const StratStat::CommonValueMap& dbEntry);
    StratStat::CommonValueMapValidationResult validateMoveDef(const StratStat::CommonValueMap& dbEntry);
    void getValidatedTeamDef(const jsonxx::Object& json);
    void transferTeamDef();
}

#endif // INNERINTERFACE_HPP
