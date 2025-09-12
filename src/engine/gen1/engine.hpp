#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "movedatabase.hpp"
#include "pkmndatabase.hpp"

class Engine
{
    private:
        PkmnDatabase pkmnDb;
        MoveDatabase moveDb;

        Engine();

    public:
        static Engine& getInstance();

        Engine(Engine const&)           = delete;
        void operator=(Engine const&)   = delete;

        const PkmnDatabase& getPkmnDb() const;
        const MoveDatabase& getMoveDb() const;

        PkmnDatabase& getPkmnDbMutable();
        MoveDatabase& getMoveDbMutable();
};

#endif // ENGINE_HPP
