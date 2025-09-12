#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>
#include <csv2/reader.hpp>

#include "pkmndatabase.hpp"
#include "movedatabase.hpp"

constexpr auto SUCCESS = 0;
constexpr auto EXPECTED_SIGNATURE = 0x504B4D4E;  // "PKMN"

enum class TextFileContentType {Integer, Double, Text};
using TextFileContentInfo = std::pair<std::string, TextFileContentType>;

extern "C" {
    int getSignature();

    // oldStyle: pass mutable ref to avoid incomplete type error of std::vector<T> in extern C
    int getPkmnDefHeaders(std::vector<TextFileContentInfo>& buffer);
    int getMoveDefHeaders(std::vector<TextFileContentInfo>& buffer);

    void connectLogger(const std::shared_ptr<spdlog::logger>& logger);
    int init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
    bool isReady();
    int shutdown();

    // init pkmn defs
    // init move defs
}

using DefaultCsvReader = csv2::Reader<
                         csv2::delimiter<','>,
                         csv2::quote_character<'"'>,
                         csv2::first_row_is_header<true>,
                         csv2::trim_policy::trim_whitespace
                         >;
const DefaultCsvReader getDefaultCsvReader();

const PkmnDatabase loadPkmnDatabase(const std::filesystem::path& pkmnDefs);
const MoveDatabase loadMoveDatabase(const std::filesystem::path& moveDefs);

#endif // INTERFACE_HPP
