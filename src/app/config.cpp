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
    return Player1Team;
}

void Config::setPlayer1Team(const std::filesystem::path& newHumanTeam)
{
    assertPathExists(PLAYER1_TEAM, newHumanTeam);
    Player1Team = newHumanTeam;
}

std::filesystem::path Config::getHumanStrat() const
{
    return Player1Strat;
}

void Config::setPlayer1Strat(const std::filesystem::path& newHumanStrat)
{
    assertPathExists(PLAYER1_STRATEGY, newHumanStrat);
    Player1Strat = newHumanStrat;
}

std::filesystem::path Config::getEnemyTeam() const
{
    return Player2Team;
}

void Config::setPlayer2Team(const std::filesystem::path& newEnemyTeam)
{
    assertPathExists(PLAYER2_TEAM, newEnemyTeam);
    Player2Team = newEnemyTeam;
}

std::filesystem::path Config::getEnemyStrat() const
{
    return Player2Strat;
}

void Config::setPlayer2Strat(const std::filesystem::path& newEnemyStrat)
{
    assertPathExists(PLAYER2_STRATEGY, newEnemyStrat);
    Player2Strat = newEnemyStrat;
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
