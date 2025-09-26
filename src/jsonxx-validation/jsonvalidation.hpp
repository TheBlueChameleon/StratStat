#ifndef JSONVALIDATION_H
#define JSONVALIDATION_H

#include <jsonxx.h>

#include "jsonvalidationtionresult.hpp"
#include "specification.hpp"

namespace JsonValidation
{
    JsonValidationResult validate(const jsonxx::Object& json, const SpecificationSet& specs);
}

#endif // JSONVALIDATION_H
