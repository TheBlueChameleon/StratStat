#ifndef BASEMOVEDEF_H
#define BASEMOVEDEF_H

#include "variantdef.hpp"

class BaseMoveDef : public VariantDef
{
    public:
        BaseMoveDef(const std::map<std::string, VariantContentType>& data);
};

#endif // BASEMOVEDEF_H
