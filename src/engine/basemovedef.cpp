#include "basemovedef.hpp"

BaseMoveDef::BaseMoveDef(
    const std::string& identifierKey,
    const std::unordered_map<std::string, VariantContentType>& data
) : VariantDef(identifierKey, data)
{}
