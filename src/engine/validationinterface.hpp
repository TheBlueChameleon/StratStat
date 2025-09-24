#ifndef VALIDATIONINTERFACE_H
#define VALIDATIONINTERFACE_H

#include <optional>

#include <jsonxx.h>

#include "sharedtypes.hpp"

namespace StratStat
{
    StratStat::CommonValueMapValidationResult validatePkmnDef(const StratStat::CommonValueMap& dbEntry);
    StratStat::CommonValueMapValidationResult validateMoveDef(const StratStat::CommonValueMap& dbEntry);

    void transferPlayerAndTeamDef(
        const jsonxx::Object& json,
        const std::string& filename,
        CommonValueMap& playerDef,
        CommonValueMapVector& teamDef
        );
}

#endif // VALIDATIONINTERFACE_H
