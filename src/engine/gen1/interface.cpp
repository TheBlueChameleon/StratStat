#include <iostream>
#include <string>
using namespace std::string_literals;

#include <engine/interface.hpp>

#include "constants.hpp"
#include "engine.hpp"
#include <engine/types.hpp>

static bool readyFlag = false;

extern "C" {

    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }

    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo(PKMN_IDENTIFIER, VariantContentID::Text),
            VariantContentInfo(PKMN_TYPE1,      VariantContentID::Text),
            VariantContentInfo(PKMN_TYPE2,      VariantContentID::Text),
            VariantContentInfo(PKMN_HP,         VariantContentID::Integer),
            VariantContentInfo(PKMN_ATK,        VariantContentID::Integer),
            VariantContentInfo(PKMN_DEF,        VariantContentID::Integer),
            VariantContentInfo(PKMN_SPC,        VariantContentID::Integer),
            VariantContentInfo(PKMN_SPD,        VariantContentID::Integer),
            VariantContentInfo(PKMN_EXPGROUP,   VariantContentID::Text),
        };
    }

    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo(MOVE_IDENTIFIER,     VariantContentID::Text),
            VariantContentInfo("type_id",           VariantContentID::Text),
            VariantContentInfo("power",             VariantContentID::Integer),
            VariantContentInfo("pp",                VariantContentID::Integer),
            VariantContentInfo("accuracy",          VariantContentID::Integer),
            VariantContentInfo("priority",          VariantContentID::Integer),
            VariantContentInfo("damage_class_id",   VariantContentID::Text),
            VariantContentInfo("effect_id",         VariantContentID::Text),
            VariantContentInfo("effect_chance",     VariantContentID::Integer),
        };
    }

    void getTeamDefStructure() {}

    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs)
    {
        spdlog::debug("INITIALIZING ENGINE");
        initPkmnDb(pkmnDefs);
        initMoveDb(moveDefs);
        readyFlag = true;
        spdlog::debug("... DONE");
    }

    bool isReady()
    {
        return readyFlag;
    }

    void shutdown()
    {
        readyFlag = false;
    }
}

void initPkmnDb(const std::filesystem::path& pkmnDefs)
{
    spdlog::trace("INITIALIZING PKMNDB");

    auto& engine = Engine::getInstance();

    auto headerRequirements = std::vector<VariantContentInfo>();
    getPkmnDefHeaders(headerRequirements);

    auto csv = DefaultCsvReader();
    csv.mmap(pkmnDefs.c_str());

    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, pkmnDefs.c_str());
    const auto expectedColumnCount = columnData.size();

    for (const auto& row: csv)
    {
        const auto pkmnData = parseCsvRow(row, columnData);
        if (pkmnData.size() == 0)
        {
            continue;
        }
        else if (pkmnData.size() < expectedColumnCount)
        {
            spdlog::error(
                "  MALFORMED PKMN DEFINITION FOR '{}' (IGNORED)",
                std::get<static_cast<int>(VariantContentID::Text)>(pkmnData.at(PKMN_IDENTIFIER)),
                pkmnData.size(),
                expectedColumnCount
            );
        }
        else
        {
            spdlog::trace(
                "  PUT SPECIES '{}' IN DB",
                std::get<static_cast<int>(VariantContentID::Text)>(pkmnData.at(PKMN_IDENTIFIER))
            );

            engine.getPkmnDbMutable().add(pkmnData);
        }
    }

    spdlog::trace("  KNOWN SPECIES: {}", engine.getPkmnDb().size());
    spdlog::trace("... SUCCESS");
}

void initMoveDb(const std::filesystem::path& moveDefs)
{
    spdlog::trace("INITIALIZING MOVEDB FROM {}", moveDefs.c_str());

    auto& engine = Engine::getInstance();

    auto headerRequirements = std::vector<VariantContentInfo>();
    getMoveDefHeaders(headerRequirements);

    auto csv = DefaultCsvReader();
    csv.mmap(moveDefs.c_str());

    const auto& header = csv.header();
    const auto columnData = analyzeHeader(header, headerRequirements, moveDefs.c_str());

    for (const auto& row: csv)
    {
        const auto moveData = parseCsvRow(row, columnData);
        if (moveData.size() == 0)
        {
            continue;
        }
        spdlog::trace(
            "  PUT MOVE '{}' IN DB",
            std::get<static_cast<int>(VariantContentID::Text)>(moveData.at(MOVE_IDENTIFIER))
        );

        engine.getMoveDbMutable().add(moveData);
    }

    spdlog::trace("  KNOWN MOVES: {}", engine.getMoveDb().size());
    spdlog::trace("... SUCCESS");
}


// this ensures "unused" functions from the static lib are still linked into the dyLib.
void dummyCalls()
{
    connectLogger(nullptr);
}
