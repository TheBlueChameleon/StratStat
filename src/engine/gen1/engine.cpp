#include "engine.hpp"

Engine::Engine() {}

Engine& Engine::getInstance()
{
    static Engine instance; // Guaranteed to be destroyed. Instantiated on first use.
    return instance;
}
