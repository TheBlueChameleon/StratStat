#include "interface.hpp"

extern "C" {
void connectLogger(const std::shared_ptr<spdlog::logger> &logger)
    {
        spdlog::set_default_logger(logger);
    }
}

const DefaultCsvReader getDefaultCsvReader()
{
    return csv2::Reader<
           csv2::delimiter<','>,
           csv2::quote_character<'"'>,
           csv2::first_row_is_header<true>,
           csv2::trim_policy::trim_whitespace
           >();
}

const PkmnDatabase loadPkmnDatabase(const std::filesystem::path& pkmnDefs)
{
    auto result = PkmnDatabase();

    return result;
}

const MoveDatabase loadMoveDatabase(const std::filesystem::path& moveDefs)
{
    auto result = MoveDatabase();

    return result;
}

