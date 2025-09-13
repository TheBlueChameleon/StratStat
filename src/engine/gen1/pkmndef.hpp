#ifndef PKMNDEF_HPP
#define PKMNDEF_HPP

#include <string>
#include <unordered_map>

#include "../basepkmndef.hpp"
#include "../types.hpp"

class PkmnDef : public BasePkmnDef
{
    public:
        PkmnDef(const std::unordered_map<std::string, VariantContentType>& data);
        PkmnDef(const BasePkmnDef& data);

        const std::string& getName() const;
        const std::string& getType1() const;
        const std::string& getType2() const;
        int getHP() const;
        int getATK() const;
        int getDEF() const;
        int getSPC() const;
        int getSPD() const;
};

#endif // PKMNDEF_HPP
