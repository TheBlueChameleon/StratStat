#include <iostream>

#include <engine/interface.hpp>

#include <plog/Log.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

extern "C" {

    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }

#define HEADER_INFO(field, type) std::make_pair(field, type)

    int getPkmnDefHeaders(std::vector<TextFileContentInfo>& buffer)
    {
        buffer =
        {
            HEADER_INFO("Name",  TextFileContentType::Text),
            HEADER_INFO("Type1", TextFileContentType::Text),
            HEADER_INFO("Type2", TextFileContentType::Text),
            HEADER_INFO("HP",    TextFileContentType::Integer),
            HEADER_INFO("ATK",   TextFileContentType::Integer),
            HEADER_INFO("DEF",   TextFileContentType::Integer),
            HEADER_INFO("SPC",   TextFileContentType::Integer),
            HEADER_INFO("SPD",   TextFileContentType::Integer),
        };
        return SUCCESS;
    }

    int getMoveDefHeaders(std::vector<TextFileContentInfo>& buffer)
    {
        buffer =
        {
            HEADER_INFO("identifier",       TextFileContentType::Text),
            HEADER_INFO("type_id",          TextFileContentType::Text),
            HEADER_INFO("power",            TextFileContentType::Integer),
            HEADER_INFO("pp",               TextFileContentType::Integer),
            HEADER_INFO("accuracy",         TextFileContentType::Integer),
            HEADER_INFO("priority",         TextFileContentType::Integer),
            HEADER_INFO("damage_class_id",  TextFileContentType::Text),
            HEADER_INFO("effect_id",        TextFileContentType::Text),
            HEADER_INFO("effect_chance",    TextFileContentType::Integer),
        };

        return SUCCESS;
    }

    int init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs)
    {
        // static plog::ColorConsoleAppender<plog::MessageOnlyFormatter> consoleAppender;
        // plog::init(plog::verbose, &consoleAppender);

        PLOG_VERBOSE << "INIT";
        std::cout << "(init cout)" << std::endl;
        return SUCCESS;
    }

    bool isReady()
    {
        return false;
    }

    int shutdown()
    {
        return SUCCESS;
    }
}
