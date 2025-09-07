#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>

struct Config
{
    struct PathEntry
    {
        const char* entryName;
        std::filesystem::path Config::* offset;
    };

    std::filesystem::path engine;
    std::filesystem::path userTeam;
    std::filesystem::path userStrat;
    std::filesystem::path enemyTeam;
    std::filesystem::path enemyStrat;
    std::filesystem::path pkmnDefs;
    std::filesystem::path moveDefs;

    int repetitions;
    // log level;
};

#endif // CONFIG_HPP
