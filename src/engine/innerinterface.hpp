#ifndef INNERINTERFACE_HPP
#define INNERINTERFACE_HPP

#include <optional>

#include <jsonxx.h>

#include "sharedtypes.hpp"

std::optional<CommonValueCollection> validatePkmnDef(const CommonValueMap& dbEntry);
std::optional<CommonValueCollection> validateMoveDef(const CommonValueMap& dbEntry);
void getValidatedTeamDef(const jsonxx::Object& json);
void transferTeamDef();

#endif // INNERINTERFACE_HPP
