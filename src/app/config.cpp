#include <iostream>

#include "config.hpp"

Config::Config() {}

void Config::assertPathExists(const std::string& attributeName, const std::filesystem::path& path) const
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "The path for '" << attributeName << "' "
                  "was specified to be '" << path << "', but no such file exists." << std::endl;
        std::exit(-1);
    }
}

std::filesystem::path Config::getEngine() const
{
    return engine;
}

void Config::setEngine(const std::filesystem::path& newEngine)
{
    assertPathExists(ENGINE, newEngine);
    engine = newEngine;
}

std::filesystem::path Config::getHumanTeam() const
{
    return humanTeam;
}

void Config::setHumanTeam(const std::filesystem::path& newHumanTeam)
{
    assertPathExists(HUMAN_TEAM, newHumanTeam);
    humanTeam = newHumanTeam;
}

std::filesystem::path Config::getHumanStrat() const
{
    return humanStrat;
}

void Config::setUserStrat(const std::filesystem::path& newHumanStrat)
{
    assertPathExists(HUMAN_STRATEGY, newHumanStrat);
    humanStrat = newHumanStrat;
}

std::filesystem::path Config::getEnemyTeam() const
{
    return enemyTeam;
}

void Config::setEnemyTeam(const std::filesystem::path& newEnemyTeam)
{
    assertPathExists(ENEMY_TEAM, newEnemyTeam);
    enemyTeam = newEnemyTeam;
}

std::filesystem::path Config::getEnemyStrat() const
{
    return enemyStrat;
}

void Config::setEnemyStrat(const std::filesystem::path& newEnemyStrat)
{
    assertPathExists(ENEMY_STRATEGY, newEnemyStrat);
    enemyStrat = newEnemyStrat;
}

std::filesystem::path Config::getPkmnDefs() const
{
    return pkmnDefs;
}

void Config::setPkmnDefs(const std::filesystem::path& newPkmnDefs)
{
    assertPathExists(PKMN_DEFS, newPkmnDefs);
    pkmnDefs = newPkmnDefs;
}

std::filesystem::path Config::getMoveDefs() const
{
    return moveDefs;
}

void Config::setMoveDefs(const std::filesystem::path& newMoveDefs)
{
    assertPathExists(MOVE_DEFS, newMoveDefs);
    moveDefs = newMoveDefs;
}

std::optional<std::filesystem::path> Config::getLogFile() const
{
    return logFile;
}

void Config::setLogFile(std::optional<std::filesystem::path> newLogFile)
{
    logFile = newLogFile;
}

int Config::getRepetitions() const
{
    return repetitions;
}

void Config::setRepetitions(int newRepetitions)
{
    if (newRepetitions < 0)
    {
        std::cerr << "Invalid number of repetitions: " << newRepetitions << std::endl;
        std::cerr << "Must be a positive number." << std::endl;
        std::exit(-1);
    }

    repetitions = newRepetitions;
}

int Config::getMaxTurns() const
{
    return maxTurns;
}

void Config::setMaxTurns(int newMaxTurns)
{
    if (newMaxTurns < 0)
    {
        std::cerr << "Invalid number of max turns per battle: " << newMaxTurns << std::endl;
        std::cerr << "Must be a positive number." << std::endl;
        std::exit(-1);
    }
    maxTurns = newMaxTurns;
}

int Config::getLogLevel() const
{
    return logLevel;
}

void Config::setLogLevel(int newLogLevel)
{
    logLevel = newLogLevel;
}
