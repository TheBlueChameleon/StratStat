#ifndef CLIPARSER_HPP
#define CLIPARSER_HPP

#include <filesystem>

#include <argparse/argparse.hpp>

#include "config.hpp"

class CliParser
{
    private:
        argparse::ArgumentParser parent;
        argparse::ArgumentParser configFileParser;
        argparse::ArgumentParser explicitParametersParser;

        void configureExplicitParametersParser();
        void configureConfigFileParser();

        const Config parseExplicit() const;
        const Config parseConfigFile() const;

        void handleErr(const std::exception& err) const;

        void validateAndTransferRepetitions(const int repetitions, Config& cfg) const;
        void validateAndTransferFile(Config::PathEntry entry, const std::string& path, Config& cfg) const;

    public:
        CliParser();

        const Config run(const int argc, const char* argv[]);
};

#endif // CLIPARSER_HPP
