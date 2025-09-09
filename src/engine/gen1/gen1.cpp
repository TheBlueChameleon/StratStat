#include <engine/interface.hpp>

extern "C" {

    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }

    int getPkmnDefHeaders(std::vector<std::string>& buffer)
    {
        buffer = {"Name","Type1","Type2","HP","ATK","DEF","SPC","SPD"};
        return SUCCESS;
    }

    int getMoveDefHeaders(std::vector<std::string>& buffer)
    {
        buffer = {"identifier","type_id","power","pp","accuracy","priority","damage_class_id","effect_id","effect_chance"};
        return SUCCESS;
    }

    int init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs)
    {
        return SUCCESS;
    }

    bool isReady()
    {
        return false;
    }

    int shutdown()
    {
        return SUCCESS;
    }
}
