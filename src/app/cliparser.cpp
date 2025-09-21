#include <iostream>
#include <fstream>
#include <exception>
#include <sstream>
#include <string>
#include <vector>
using namespace std::string_literals;

#include <jsonxx.h>
#include <argparse/argparse.hpp>
using ArgParser = argparse::ArgumentParser;

#include "cliparser.hpp"
#include "config.hpp"

#include "../json-schema/jsonvalidation.hpp"

auto mandatoryPaths =
{
    Config::PathEntry(ENGINE, &Config::setEngine),
    Config::PathEntry(PLAYER1_TEAM, &Config::setPlayer1Team),
    Config::PathEntry(PLAYER1_STRATEGY, &Config::setPlayer1Strat),
    Config::PathEntry(PLAYER2_TEAM, &Config::setPlayer2Team),
    Config::PathEntry(PLAYER2_STRATEGY, &Config::setPlayer2Strat),
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
    .add_argument(PLAYER1_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    explicitParametersParser
    .add_argument(PLAYER1_STRATEGY)
    .help("The file path of a user strategy definition file (LUA file)");

    explicitParametersParser
    .add_argument(PLAYER2_TEAM)
    .help("The file path of a user team definition file (JSON file)");

    explicitParametersParser
    .add_argument(PLAYER2_STRATEGY)
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

    validateConfigFile(json, cfgFileName);

    for (const auto& cfgInfo : mandatoryPaths)
    {
        const std::string attributeName = cfgInfo.attributeName;
        const auto setter = cfgInfo.setter;
        const auto value = json.get<std::string>(attributeName);
        (cfg.*setter)(value);
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

void CliParser::validateConfigFile(const jsonxx::Object& json, const std::filesystem::path& cfgFileName) const
{
    JsonValidation::SpecificationSet specs;

    specs.emplace(ENGINE, JsonValidation::TID_String, true);
    specs.emplace(PLAYER1_TEAM, JsonValidation::TID_String, true);
    specs.emplace(PLAYER1_STRATEGY, JsonValidation::TID_String, true);
    specs.emplace(PLAYER2_TEAM, JsonValidation::TID_String, true);
    specs.emplace(PLAYER2_STRATEGY, JsonValidation::TID_String, true);
    specs.emplace(PKMN_DEFS, JsonValidation::TID_String, true);
    specs.emplace(MOVE_DEFS, JsonValidation::TID_String, true);
    specs.emplace(REPETITIONS, JsonValidation::TID_Number, false);
    specs.emplace(MAX_TURNS, JsonValidation::TID_Number, false);
    specs.emplace(LOGFILE, JsonValidation::TID_String, false);
    specs.emplace(LOGLEVEL, JsonValidation::TID_Number, false);

    const auto result = JsonValidation::validate(json, specs);

    if (result.hasMessages())
    {
        std::cerr << "Validation result of " << cfgFileName.c_str() << ":" << std::endl;
    }

    for (const auto& msg: result.getValidationWarnings())
    {
        std::cerr << "WARNING: " << msg << std::endl;
    }

    if (!result.isValid())
    {
        for (const auto& msg: result.getValidationErrors())
        {
            std::cerr << "ERROR: " << msg << std::endl;
        }
        std::exit(0);
    }
}
