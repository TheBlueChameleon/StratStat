#ifndef BASEMOVEDEF_H
#define BASEMOVEDEF_H

#include "variantdef.hpp"

class BaseMoveDef : public VariantDef
{
    public:
        BaseMoveDef(const std::unordered_map<std::string, VariantContentType>& data);
};

#endif // BASEMOVEDEF_H
