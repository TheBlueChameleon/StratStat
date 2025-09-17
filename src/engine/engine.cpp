#include <iostream>

#include "engine.hpp"

Engine::Engine()
{
    std::cout << "CTOR ENGINE: " << this << std::endl;
}

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
