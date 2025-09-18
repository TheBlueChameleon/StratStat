#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <unordered_map>
#include <string>

#include "commondatabase.hpp"

class Engine
{
    private:
        CommonDatabase pkmnDb;
        CommonDatabase moveDb;

        CommonValueMap player1;
        CommonValueMap player2;

        CommonValueMapVector team1;
        CommonValueMapVector team2;

        Engine();

    public:
        static Engine& getInstance();

        Engine(Engine const&)           = delete;
        void operator=(Engine const&)   = delete;

        const CommonDatabase& getPkmnDb() const;
        const CommonDatabase& getMoveDb() const;

        CommonDatabase& getPkmnDbMutable();
        CommonDatabase& getMoveDbMutable();

        const CommonValueMap& getPlayer1() const;
        const CommonValueMap& getPlayer2() const;

        CommonValueMap& getPlayer1Mutable();
        CommonValueMap& getPlayer2Mutable();

        const CommonValueMapVector& getTeam1() const;
        const CommonValueMapVector& getTeam2() const;

        CommonValueMapVector& getTeam1Mutable();
        CommonValueMapVector& getTeam2Mutable();
};

Engine& getEngineInstance();

#endif // ENGINE_HPP
