#ifndef INNERINTERFACE_HPP
#define INNERINTERFACE_HPP

#include <jsonxx.h>

void foo();
void validatePkmnDb();
void validateMoveDb();
void validateTeamDef(const jsonxx::Object& json);
void transferTeamDef();

#endif // INNERINTERFACE_HPP
