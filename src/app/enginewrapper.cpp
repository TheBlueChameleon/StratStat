#include <iostream>

#include <spdlog/spdlog.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <engine/interface.hpp>
#include "enginewrapper.hpp"

// TODO test win version of this

void EngineWrapper::loadEninge(const std::filesystem::path& enginePath)
{
    spdlog::trace("LOADING ENGINE FROM {}", enginePath.c_str());

#ifdef _WIN32
    handler = LoadLibrary(enginePath.c_str());
#else
    handler = dlopen(enginePath.c_str(), RTLD_LAZY);
#endif

    if (!handler)
    {
        spdlog::critical("COULD NOT LOAD {}", enginePath.c_str());
        spdlog::critical(dlerror());
        std::exit(-1);
    }

    spdlog::trace("... SUCCESS!");
}

#define FETCH(symbol) fetchCheckAndTransfer(&EngineWrapper::_##symbol, #symbol)

template<typename T>
void EngineWrapper::fetchCheckAndTransfer(T EngineWrapper::* offset, const char* const symbol)
{
    using targetPtr_t = std::remove_reference<decltype(this->*offset)>::type;
    targetPtr_t target = reinterpret_cast<targetPtr_t>(findSymbol(symbol));

    if (target == nullptr)
    {
        spdlog::critical("COULD NOT EXTRACT FUNCTION {}", symbol);
        std::exit(-1);
    }
    else
    {
        spdlog::trace("  ... EXTRACTED {}", symbol);
    }

    this->*offset =target;
}

void EngineWrapper::testSignature()
{
    spdlog::trace("TESTING FOR SIGNATURE ...");
    FETCH(getSignature);

    if (getSignature() != EXPECTED_SIGNATURE)
    {
        spdlog::critical("UNEXPECTED SIGNATURE");
        std::exit(-1);
    }
    spdlog::trace("... SUCCESS!");
}

void EngineWrapper::extractFunctions()
{
    spdlog::trace("EXTRACTING FUNCTIONS ...");

    FETCH(getTypeNames);
    FETCH(getPkmnDefHeaders);
    FETCH(getMoveDefHeaders);
    FETCH(getTeamDefStructure);
    FETCH(init);
    FETCH(loadTeams);
    FETCH(connectLogger);
    FETCH(shutdown);
    FETCH(isReady);

    spdlog::trace("... SUCCESS!");
}

EngineWrapper::EngineWrapper(const std::filesystem::path& enginePath)
{
    spdlog::debug("BOOTING ENGINE WRAPPER");

    loadEninge(enginePath);
    testSignature();
    extractFunctions();

    spdlog::debug("... DONE");
}

EngineWrapper::~EngineWrapper()
{
    if (handler)
    {
#ifdef _WIN32
        FreeLibrary(handler);
#else
        dlclose(handler);
#endif
    }
}

void* EngineWrapper::findSymbol(const char* const symbolName)
{
#ifdef _WIN32
    auto* symbol = reinterpret_cast<void*>(GetProcAddress(hGetProcIDDLL, symbolName));
    if (!symbol)
    {
        spdlog::critical("COULD NOT FIND SYMBOL {}", symbolName);
    }
#else
    char* error = nullptr;
    auto* symbol = dlsym(handler, symbolName);
    if ((error = dlerror()) != nullptr || !symbol)
    {
        spdlog::critical("COULD NOT FIND SYMBOL {}", symbolName);
        spdlog::critical(error);
        return nullptr;
    }
#endif
    return symbol;
}

int EngineWrapper::getSignature() const
{
    return _getSignature();
}

void EngineWrapper::getTypeNames(JsonValidation::AllowedValues& buffer) const
{
    return _getTypeNames(buffer);
}

void EngineWrapper::getPkmnDefHeaders(std::vector<StratStat::VariantContentInfo>& buffer) const
{
    return _getPkmnDefHeaders(buffer);
}

void EngineWrapper::getMoveDefHeaders(std::vector<StratStat::VariantContentInfo>& buffer) const
{
    return _getMoveDefHeaders(buffer);
}

void EngineWrapper::getTeamDefStructure(JsonValidation::SpecificationSet& specs) const
{
    return _getTeamDefStructure(specs);
}

void EngineWrapper::init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs) const
{
    return _init(pkmnDefs, moveDefs);
}

void EngineWrapper::loadTeams(const std::filesystem::path& player1TeamDef, const std::filesystem::path& player2TeamDef) const
{
    return _loadTeams(player1TeamDef, player2TeamDef);
}

void EngineWrapper::shutdown() const
{
    return _shutdown();
}

bool EngineWrapper::isReady() const
{
    return _isReady();
}

void EngineWrapper::connectLogger(const std::shared_ptr<spdlog::logger>& logger) const
{
    return _connectLogger(logger);
}
