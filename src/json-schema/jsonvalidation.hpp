#ifndef JSONVALIDATION_H
#define JSONVALIDATION_H

#include <jsonxx.h>

#include "jsonvalidationtionresult.hpp"
#include "node.hpp"

namespace JsonValidation
{
    JsonValidationResult validate(const jsonxx::Object& json, const std::unordered_set<Node>& specs);
}

#endif // JSONVALIDATION_H
