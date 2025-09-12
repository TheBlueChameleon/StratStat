#include <iostream>

#include "basepkmndef.hpp"

BasePkmnDef::BasePkmnDef(
    const std::string& identifierKey,
    const std::unordered_map<std::string, VariantContentType>& data
) : VariantDef(identifierKey, data)
{}
