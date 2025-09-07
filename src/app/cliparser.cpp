#include <fstream>
#include <exception>
#include <sstream>
#include <string>
using namespace std::string_literals;

#include <jsonxx.h>
#include <argparse/argparse.hpp>
using ArgParser = argparse::ArgumentParser;

#include "cliparser.hpp"
#include "config.hpp"

auto mandatoryPaths =
{
    Config::PathEntry(ENGINE, &Config::setEngine),
    Config::PathEntry(HUMAN_TEAM, &Config::setHumanTeam),
    Config::PathEntry(HUMAN_STRATEGY, &Config::setUserStrat),
    Config::PathEntry(ENEMY_TEAM, &Config::setEnemyTeam),
    Config::PathEntry(ENEMY_STRATEGY, &Config::setEnemyStrat),
    Config::PathEntry(PKMN_DEFS, &Config::setPkmnDefs),
    Config::PathEntry(MOVE_DEFS, &Config::setMoveDefs)
};

auto optionalPaths =
{
    Config::OptPathEntry(LOGFILE, &Config::setLogFile),
};

auto intParams =
{
    Config::IntEntry(REPETITIONS, &Config::setRepetitions),
    Config::IntEntry(MAX_TURNS, &Config::setMaxTurns),
    Config::IntEntry(LOGLEVEL, &Config::setLogLevel),
};

CliParser::CliParser() :
    parent(ArgParser(APP_NAME, APP_VERSION)),
    configFileParser(ArgParser(CONFIG_FILE)),
    explicitParametersParser(ArgParser(EXPLICIT))
{
    configureExplicitParametersParser();
    configureConfigFileParser();

    parent.add_subparser(configFileParser);
    parent.add_subparser(explicitParametersParser);
}


void CliParser::configureExplicitParametersParser()
{
    explicitParametersParser
    .add_argument(ENGINE)
    .help("The file path of a BattleEngine library file (DLL/SO file)");

    explicitParametersParser
    .add_argument(HUMAN_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    explicitParametersParser
    .add_argument(HUMAN_STRATEGY)
    .help("The file path of a user strategy definition file (LUA file)");

    explicitParametersParser
    .add_argument(ENEMY_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    explicitParametersParser
    .add_argument(ENEMY_STRATEGY)
    .help("The file path of a user strategy definition file (LUA file)");

    explicitParametersParser
    .add_argument(PKMN_DEFS)
    .help("The file path of a Pokémon definition file (CSV file)");

    explicitParametersParser
    .add_argument(MOVE_DEFS)
    .help("The file path of a attack definition file (CSV file)");

    explicitParametersParser
    .add_argument("--"s + LOGFILE)
    .help("The file path write logs into.");

    explicitParametersParser
    .add_argument("--"s + LOGLEVEL)
    .help("The minimum importance of messages to appear in a log. Must be between 0 and 6. (default: 4)")
    .scan<'i', int>()
    .default_value(REPETITIONS_DEFAULT);

    explicitParametersParser
    .add_argument("--"s + REPETITIONS)
    .help("The number of battles the engine attempts with the specified teams (default: 10)")
    .scan<'i', int>()
    .default_value(REPETITIONS_DEFAULT);

    explicitParametersParser
    .add_argument("--"s + MAX_TURNS)
    .help("The maximum number of turns a battle may last (default: 100)")
    .scan<'i', int>()
    .default_value(MAX_TURNS_DEFAULT);
}

void CliParser::configureConfigFileParser()
{
    configFileParser
    .add_argument(CONFIG_FILE_NAME)
    .help("The file path of a StatStrat parameter definition file (JSON file)");
}

const Config CliParser::run(const int argc, const char* argv[])
{
    if (argc == 1)
    {
        std::cerr << parent << std::endl;
        std::exit(-1);
    }

    try
    {
        parent.parse_args(argc, argv);
    }
    catch (const std::exception& err)
    {
        handleErr(err);
    }

    if (parent.is_subcommand_used(explicitParametersParser))
    {
        return parseExplicit();
    }
    else if (parent.is_subcommand_used(configFileParser))
    {
        return parseConfigFile();
    }
    else
    {
        throw std::runtime_error("Illegal State: command unknown");
    }
}

void CliParser::handleErr(const std::exception& err) const
{
    std::cerr << err.what() << std::endl;

    if (parent.is_subcommand_used(explicitParametersParser))
    {
        std::cerr << explicitParametersParser << std::endl;
    }
    else if (parent.is_subcommand_used(configFileParser))
    {
        std::cerr << configFileParser << std::endl;
    }
    else
    {
        throw std::runtime_error("Illegal State: command unknown");
    }

    std::exit(-1);
}

const Config CliParser::parseExplicit() const
{
    auto cfg = Config();

    for (const auto cfgInfo: mandatoryPaths)
    {
        const auto setter = cfgInfo.setter;
        const auto value = explicitParametersParser.get<std::string>(cfgInfo.attributeName);
        (cfg.*setter)(value);
    }

    for (const auto cfgInfo: optionalPaths)
    {
        const auto setter = cfgInfo.setter;
        const auto value = explicitParametersParser.present<std::string>(cfgInfo.attributeName);
        (cfg.*setter)(value);
    }

    for (const auto cfgInfo: intParams)
    {
        const auto setter = cfgInfo.setter;
        const auto value = explicitParametersParser.get<int>(cfgInfo.attributeName);
        (cfg.*setter)(value);
    }

    return cfg;
}

const Config CliParser::parseConfigFile() const
{
    auto cfg = Config();
    std::filesystem::path cfgFileName = configFileParser.get<std::string>(CONFIG_FILE_NAME);

    if (!std::filesystem::exists(cfgFileName))
    {
        std::cerr << "The config file " << cfgFileName << " does not exist." << std::endl;
        std::exit(-1);
    }

    auto cfgFile = std::ifstream(cfgFileName);
    jsonxx::Object json;
    const bool success = json.parse(cfgFile);
    if (!success)
    {
        std::cerr << "The config file " << cfgFileName << " is not a valid JSON file." << std::endl;
        std::exit(-1);
    }

    for (const auto& cfgInfo : mandatoryPaths)
    {
        const std::string attributeName = cfgInfo.attributeName;
        const auto setter = cfgInfo.setter;

        if (!json.has<std::string>(attributeName))
        {
            std::cerr << "The required path '" << attributeName << "' was not found in the cfgFile." << std::endl;
            std::exit(-1);
        }
        else
        {
            const auto value = json.get<std::string>(attributeName);
            (cfg.*setter)(value);
        }
    }

    for (const auto& cfgInfo : optionalPaths)
    {
        const std::string attributeName = cfgInfo.attributeName;
        const auto setter = cfgInfo.setter;

        if (json.has<jsonxx::String>(attributeName))
        {
            const auto value = json.get<std::string>(attributeName);
            (cfg.*setter)(value);
        }
    }

    for (const auto& cfgInfo : intParams)
    {
        const std::string attributeName = cfgInfo.attributeName;
        const auto setter = cfgInfo.setter;

        if (json.has<jsonxx::Number>(attributeName))
        {
            const auto value = json.get<jsonxx::Number>(attributeName);
            (cfg.*setter)(value);
        }
    }

    return cfg;
}

