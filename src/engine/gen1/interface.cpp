#include <iostream>

#include <engine/interface.hpp>

#include "engine.hpp"

extern "C" {

    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }

    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo("Name",  VariantContentID::Text),
            VariantContentInfo("Type1", VariantContentID::Text),
            VariantContentInfo("Type2", VariantContentID::Text),
            VariantContentInfo("HP",    VariantContentID::Integer),
            VariantContentInfo("ATK",   VariantContentID::Integer),
            VariantContentInfo("DEF",   VariantContentID::Integer),
            VariantContentInfo("SPC",   VariantContentID::Integer),
            VariantContentInfo("SPD",   VariantContentID::Integer),
        };
    }

    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer)
    {
        buffer =
        {
            VariantContentInfo("identifier",       VariantContentID::Text),
            VariantContentInfo("type_id",          VariantContentID::Text),
            VariantContentInfo("power",            VariantContentID::Integer),
            VariantContentInfo("pp",               VariantContentID::Integer),
            VariantContentInfo("accuracy",         VariantContentID::Integer),
            VariantContentInfo("priority",         VariantContentID::Integer),
            VariantContentInfo("damage_class_id",  VariantContentID::Text),
            VariantContentInfo("effect_id",        VariantContentID::Text),
            VariantContentInfo("effect_chance",    VariantContentID::Integer),
        };
    }

    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs)
    {
        spdlog::debug("INITIALIZING ENGINE");

        initPkmnDb(pkmnDefs);
        initMoveDb(moveDefs);
        spdlog::debug("... DONE");
    }

    bool isReady()
    {
        return false;
    }

    void shutdown() {}
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

    for (const auto& row: csv)
    {
        const auto pkmnData = parseCsvRow(row, columnData);

        engine.getPkmnDbMutable().add(pkmnData);
    }

    spdlog::trace("... SUCCESS");
}

void initMoveDb(const std::filesystem::path& pkmnDefs)
{
    spdlog::trace("INITIALIZING MOVEDB");
    spdlog::trace("... SUCCESS");
}


// this ensures "unused" functions from the static lib are still linked into the dyLib.
void dummyCalls()
{
    connectLogger(nullptr);
}
