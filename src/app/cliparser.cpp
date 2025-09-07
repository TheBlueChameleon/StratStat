#include <fstream>
#include <exception>
#include <sstream>
#include <string>
using namespace std::string_literals;

#include <jsonxx.h>

#include "cliparser.hpp"

using ArgParser = argparse::ArgumentParser;

constexpr auto APP_NAME = "StratStat";

constexpr auto CONFIG_FILE = "configFile";
constexpr auto EXPLICIT = "explicit";

constexpr auto CONFIG_FILE_NAME = "file";

constexpr auto ENGINE = "engine";
constexpr auto HUMAN_TEAM = "humanTeam";
constexpr auto HUMAN_STRATEGY = "humanStrategy";
constexpr auto ENEMY_TEAM = "enemyTeam";
constexpr auto ENEMY_STRATEGY = "enemyStrategy";
constexpr auto PKMN_DEFS = "pkmnDefs";
constexpr auto MOVE_DEFS= "moveDefs";
constexpr auto REPETITIONS = "repetitions";

constexpr auto REPETITIONS_DEFAULT = 10;

auto fileSlots =
{
    Config::PathEntry(ENGINE, &Config::engine),
    Config::PathEntry(HUMAN_TEAM, &Config::userTeam),
    Config::PathEntry(HUMAN_STRATEGY, &Config::userStrat),
    Config::PathEntry(ENEMY_TEAM, &Config::enemyTeam),
    Config::PathEntry(ENEMY_STRATEGY, &Config::enemyStrat),
    Config::PathEntry(PKMN_DEFS, &Config::pkmnDefs),
    Config::PathEntry(MOVE_DEFS, &Config::moveDefs)
};

CliParser::CliParser() :
    parent(ArgParser(APP_NAME)),
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
    .add_argument("--"s + REPETITIONS)
    .help("The number of battles the engine attempts with the specified teams (default: 10)")
    .scan<'i', int>()
    .default_value(REPETITIONS_DEFAULT);
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

    for (const auto slot: fileSlots)
    {
        validateAndTransferFile(slot, explicitParametersParser.get<std::string>(slot.entryName), cfg);
    }

    validateAndTransferRepetitions(explicitParametersParser.get<int>(REPETITIONS), cfg);

    return cfg;
}

const Config CliParser::parseConfigFile() const
{
    auto cfg = Config();
    std::filesystem::path cfgFile = configFileParser.get<std::string>(CONFIG_FILE_NAME);

    if (!std::filesystem::exists(cfgFile))
    {
        std::cerr << "The config file " << cfgFile << " does not exist." << std::endl;
        std::exit(-1);
    }

    auto file = std::ifstream(cfgFile);
    jsonxx::Object json;
    const bool success = json.parse(file);
    if (!success)
    {
        std::cerr << "The config file " << cfgFile << " is not a valid JSON file." << std::endl;
        std::exit(-1);
    }


    for (const auto& slot : fileSlots)
    {
        const std::string slotName = slot.entryName;
        if (!json.has<std::string>(slotName))
        {
            std::cerr << "The required path '" << slotName << "' was not found in the cfgFile." << std::endl;
            std::exit(-1);
        }
        else
        {
            validateAndTransferFile(slot, json.get<std::string>(slotName), cfg);
        }
    }

    if (json.has<jsonxx::Number>(REPETITIONS))
    {
        validateAndTransferRepetitions(json.get<jsonxx::Number>(REPETITIONS), cfg);
    }
    else
    {
        cfg.repetitions = REPETITIONS_DEFAULT;
    }

    return cfg;
}

void CliParser::validateAndTransferFile(Config::PathEntry entry, const std::string& path, Config& cfg) const
{
    const auto field = entry.entryName;
    const auto offset = entry.offset;

    if (std::filesystem::exists(path))
    {
        cfg.*offset = path;
    }
    else
    {
        std::cerr << "The required parameter '" << field << "' "
                  "was specified to be '" << path << "', but no such file exists." << std::endl;
        std::exit(-1);
    }
}

void CliParser::validateAndTransferRepetitions(const int repetitions, Config& cfg) const
{
    if (repetitions > 0)
    {
        cfg.repetitions = repetitions;
    }
    else
    {
        std::cerr << "Invalid number of repetitions: " << repetitions << std::endl;
        std::cerr << "Must be a positive number." << std::endl;
        std::exit(-1);
    }
}
