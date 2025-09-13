#include "constants.hpp"
#include "movedef.hpp"

MoveDef::MoveDef(const std::unordered_map<std::string, VariantContentType>& data) :
    BaseMoveDef(MOVE_IDENTIFIER, data)
{}

