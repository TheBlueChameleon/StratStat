#ifndef CLIPARSER_HPP
#define CLIPARSER_HPP

#include <filesystem>

#include <argparse/argparse.hpp>
#include <jsonxx.h>

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
        void validateConfigFile(const jsonxx::Object& json, const std::filesystem::path& cfgFileName) const;

        void handleErr(const std::exception& err) const;

    public:
        CliParser();

        const Config run(const int argc, const char* argv[]);
};

#endif // CLIPARSER_HPP
