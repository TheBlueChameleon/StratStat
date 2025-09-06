#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>

struct Config
{
    std::filesystem::path engine;
    std::filesystem::path userTeam;
    std::filesystem::path userStrat;
    std::filesystem::path enemyTeam;
    std::filesystem::path enemyStrat;
};

#endif // CONFIG_HPP
