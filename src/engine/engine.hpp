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

        std::unordered_map<std::string, VariantContentType> player1;
        std::unordered_map<std::string, VariantContentType> player2;

        Engine();

    public:
        static Engine& getInstance();

        Engine(Engine const&)           = delete;
        void operator=(Engine const&)   = delete;

        const CommonDatabase& getPkmnDb() const;
        const CommonDatabase& getMoveDb() const;

        CommonDatabase& getPkmnDbMutable();
        CommonDatabase& getMoveDbMutable();
};

Engine& getEngineInstance();

#endif // ENGINE_HPP
