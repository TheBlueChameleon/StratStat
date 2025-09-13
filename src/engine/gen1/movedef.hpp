#ifndef MOVEDEF_HPP
#define MOVEDEF_HPP

#include <engine/basemovedef.hpp>


class MoveDef : public BaseMoveDef
{
    public:
        MoveDef(const std::unordered_map<std::string, VariantContentType>& data);
};

#endif // MOVEDEF_HPP
