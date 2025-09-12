#include "engine.hpp"

Engine::Engine() {}

Engine& Engine::getInstance()
{
    static Engine instance; // Guaranteed to be destroyed. Instantiated on first use.
    return instance;
}

const PkmnDatabase& Engine::getPkmnDb() const
{
    return pkmnDb;
}

const MoveDatabase& Engine::getMoveDb() const
{
    return moveDb;
}

PkmnDatabase& Engine::getPkmnDbMutable()
{
    return pkmnDb;
}

MoveDatabase &Engine::getMoveDbMutable()
{
    return moveDb;
}
