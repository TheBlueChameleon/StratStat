#include "engine.hpp"

namespace StratStat
{
    Engine::Engine()
    {}

    Engine& Engine::getInstance()
    {
        static Engine instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    const CommonDatabase& Engine::getPkmnDb() const
    {
        return pkmnDb;
    }

    const CommonDatabase& Engine::getMoveDb() const
    {
        return moveDb;
    }

    CommonDatabase& Engine::getPkmnDbMutable()
    {
        return pkmnDb;
    }

    CommonDatabase& Engine::getMoveDbMutable()
    {
        return moveDb;
    }

    const CommonValueMap& Engine::getPlayer1() const
    {
        return player1;
    }

    const CommonValueMap& Engine::getPlayer2() const
    {
        return player2;
    }

    CommonValueMap& Engine::getPlayer1Mutable()
    {
        return player1;
    }

    CommonValueMap& Engine::getPlayer2Mutable()
    {
        return player2;
    }

    const CommonValueMapVector& Engine::getTeam1() const
    {
        return team1;
    }

    const CommonValueMapVector& Engine::getTeam2() const
    {
        return team2;
    }

    CommonValueMapVector& Engine::getTeam1Mutable()
    {
        return team1;
    }

    CommonValueMapVector& Engine::getTeam2Mutable()
    {
        return team2;
    }
}
