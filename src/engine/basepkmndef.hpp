#ifndef BASEPKMNDEF_H
#define BASEPKMNDEF_H

#include <unordered_map>
#include <string>
#include <variant>

#include "types.hpp"
#include "variantdef.hpp"

class BasePkmnDef : public VariantDef
{
    public:
        BasePkmnDef(const std::unordered_map<std::string, VariantContentType>& data);
};

#endif // BASEPKMNDEF_H
