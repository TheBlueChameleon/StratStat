#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <filesystem>
#include <string>
#include <vector>

constexpr auto SUCCESS = 0;
constexpr auto EXPECTED_SIGNATURE = 0xDEADBEEF;

extern "C" {
    int getSignature();

    int getPkmnDefHeaders(std::vector<std::string>& buffer);
    int getMoveDefHeaders(std::vector<std::string>& buffer);

    int init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
    bool isReady();
    int shutdown();

    // init pkmn defs
    // init move defs
}

#endif // INTERFACE_HPP
