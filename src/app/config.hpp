#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>
#include <optional>

constexpr auto APP_NAME = "StratStat";
constexpr auto APP_VERSION = "0.1";

constexpr auto CONFIG_FILE = "configFile";
constexpr auto EXPLICIT = "explicit";

constexpr auto CONFIG_FILE_NAME = "file";

constexpr auto ENGINE = "engine";
constexpr auto HUMAN_TEAM = "humanTeam";
constexpr auto HUMAN_STRATEGY = "humanStrategy";
constexpr auto ENEMY_TEAM = "enemyTeam";
constexpr auto ENEMY_STRATEGY = "enemyStrategy";
constexpr auto PKMN_DEFS = "pkmnDefs";
constexpr auto MOVE_DEFS= "moveDefs";
constexpr auto REPETITIONS = "repetitions";
constexpr auto MAX_TURNS = "maxTurns";
constexpr auto LOGFILE = "logfile";
constexpr auto LOGLEVEL = "loglevel";

constexpr auto REPETITIONS_DEFAULT = 10;
constexpr auto MAX_TURNS_DEFAULT = 100;
constexpr auto LOGLEVEL_DEFAULT = 4;

class Config
{
    public:
        struct PathEntry
        {
            const char* attributeName;
            void (Config::* setter)(const std::filesystem::path&) ;
        };

        struct OptPathEntry
        {
            const char* attributeName;
            void (Config::* setter)(std::optional<std::filesystem::path>) ;
        };

        struct IntEntry
        {
            const char* attributeName;
            void (Config::* setter)(int) ;
        };

    private:
        std::filesystem::path engine;
        std::filesystem::path humanTeam;
        std::filesystem::path humanStrat;
        std::filesystem::path enemyTeam;
        std::filesystem::path enemyStrat;
        std::filesystem::path pkmnDefs;
        std::filesystem::path moveDefs;

        std::optional<std::filesystem::path> logFile;

        int repetitions = REPETITIONS_DEFAULT;
        int maxTurns = MAX_TURNS_DEFAULT;
        int logLevel = LOGLEVEL_DEFAULT;

        void assertPathExists(const std::string& attributeName, const std::filesystem::path& path) const;

    public:
        Config();

        std::filesystem::path getEngine() const;
        void setEngine(const std::filesystem::path& newEngine);

        std::filesystem::path getHumanTeam() const;
        void setHumanTeam(const std::filesystem::path& newHumanTeam);

        std::filesystem::path getHumanStrat() const;
        void setUserStrat(const std::filesystem::path& newHumanStrat);

        std::filesystem::path getEnemyTeam() const;
        void setEnemyTeam(const std::filesystem::path& newEnemyTeam);

        std::filesystem::path getEnemyStrat() const;
        void setEnemyStrat(const std::filesystem::path& newEnemyStrat);

        std::filesystem::path getPkmnDefs() const;
        void setPkmnDefs(const std::filesystem::path& newPkmnDefs);

        std::filesystem::path getMoveDefs() const;
        void setMoveDefs(const std::filesystem::path& newMoveDefs);

        std::optional<std::filesystem::path> getLogFile() const;
        void setLogFile(std::optional<std::filesystem::path> newLogFile);

        int getRepetitions() const;
        void setRepetitions(int newRepetitions);

        int getMaxTurns() const;
        void setMaxTurns(int newMaxTurns);

        int getLogLevel() const;
        void setLogLevel(int newLogLevel);
};

#endif // CONFIG_HPP
