#include <argparse/argparse.hpp>
using ArgParser = argparse::ArgumentParser;

#include "clihandler.hpp"

constexpr auto ENGINE = "engine";
constexpr auto USER_TEAM = "userTeam";
constexpr auto USER_STRATEGY = "userStrategy";
constexpr auto ENEMY_TEAM = "enemyTeam";
constexpr auto ENEMY_STRATEGY = "enemyStrategy";

constexpr auto fileFields =
{
    std::make_pair(ENGINE, &Config::engine),
    std::make_pair(USER_TEAM, &Config::userTeam),
    std::make_pair(USER_STRATEGY, &Config::userStrat),
    std::make_pair(ENEMY_TEAM, &Config::enemyTeam),
    std::make_pair(ENEMY_STRATEGY, &Config::enemyStrat)
};


void configureParser(ArgParser& parser)
{
    parser
    .add_argument(ENGINE)
    .help("The file path of a BattleEngine library file (DLL/SO file)");

    parser
    .add_argument(USER_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    parser
    .add_argument(USER_STRATEGY)
    .help("The file path of a user strategy definition file (LUA file)");

    parser
    .add_argument(ENEMY_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    parser
    .add_argument(ENEMY_STRATEGY)
    .help("The file path of a user strategy definition file (LUA file)");

    parser
    .add_argument("--repetitions", "-r")
    .help("The number of battles the engine attempts with the specified teams (default: 10)")
    .default_value(10);
}

void parseOrExit(ArgParser& parser, int argc, char* argv[])
{
    try
    {
        parser.parse_args(argc, argv);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(-1);
    }
}

void validateAndTransferFile(const std::pair<const char*, std::filesystem::path Config::*> def, const ArgParser& parser, Config& cfg)
{
    const auto field = def.first;
    const auto offset = def.second;

    const auto path = cfg.*offset = parser.get<std::string>(field);

    if (!std::filesystem::exists(path))
    {
        std::cerr << "The required parameter '" << field << "' "
                  "was specified to be '" << path << "', but no such file exists." << std::endl;
        std::exit(-1);
    }

}

Config parseCliArgs(int argc, char* argv[])
{
    auto cfg = Config();
    auto parser = ArgParser("StratStat");
    configureParser(parser);
    parseOrExit(parser, argc, argv);

    for (const auto field : fileFields)
    {
        validateAndTransferFile(field, parser, cfg);
    }

    return cfg;
}
