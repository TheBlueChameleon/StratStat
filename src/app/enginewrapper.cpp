#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <plog/Log.h>

#include <engine/interface.hpp>
#include "enginewrapper.hpp"

// TODO test win version of this

void EngineWrapper::loadEninge(const std::filesystem::path& enginePath)
{
    PLOG_VERBOSE << "LOADING ENGINE FROM " << enginePath << "...";

#ifdef _WIN32
    handler = LoadLibrary(enginePath.c_str());
#else
    handler = dlopen(enginePath.c_str(), RTLD_LAZY);
#endif

    if (!handler)
    {
        PLOG_FATAL << "COULD NOT LOAD " << enginePath;
        PLOG_FATAL << dlerror();
        std::exit(-1);
    }

    PLOG_VERBOSE << "  ... SUCCESS!";
}

#define FETCH(symbol) fetchCheckAndTransfer(&EngineWrapper::_##symbol, #symbol)

template<typename T>
void EngineWrapper::fetchCheckAndTransfer(T EngineWrapper::* offset, const char* const symbol)
{
    using targetPtr_t = std::remove_reference<decltype(this->*offset)>::type;
    targetPtr_t target = reinterpret_cast<targetPtr_t>(findSymbol(symbol));

    if (target == nullptr)
    {
        PLOG_FATAL << "COULD NOT EXTRACT FUNCTION " << symbol;
        std::exit(-1);
    }
    else
    {
        PLOG_VERBOSE << "    ... EXTRACTED " << symbol;
    }

    this->*offset =target;
}

void EngineWrapper::testSignature()
{
    PLOG_VERBOSE << "TESTING FOR SIGNATURE ...";
    FETCH(getSignature);

    if (getSignature() != EXPECTED_SIGNATURE)
    {
        PLOG_FATAL << "UNEXPECTED SIGNATURE";
        std::exit(-1);
    }
    PLOG_VERBOSE << "  ... SUCCESS!";
}

void EngineWrapper::extractFunctions()
{
    PLOG_VERBOSE << "EXTRACTING FUNCTIONS ...";

    FETCH(getPkmnDefHeaders);
    FETCH(getMoveDefHeaders);
    FETCH(init);
    FETCH(shutdown);
    FETCH(isReady);

    PLOG_VERBOSE << "  ... SUCCESS!";
}

EngineWrapper::EngineWrapper(const std::filesystem::path& enginePath)
{
    loadEninge(enginePath);
    testSignature();
    extractFunctions();
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
        PLOG_ERROR << "COULD NOT FIND SYMBOL " << symbolName;
    }
#else
    char* error = nullptr;
    auto* symbol = dlsym(handler, symbolName);
    if ((error = dlerror()) != nullptr || !symbol)
    {
        PLOG_ERROR << "COULD NOT FIND SYMBOL " << symbolName;
        PLOG_ERROR << error;
        return nullptr;
    }
#endif
    return symbol;
}

int EngineWrapper::getSignature() const
{
    return _getSignature();
}

int EngineWrapper::getPkmnDefHeaders(std::vector<TextFileContentInfo>& buffer) const
{
    return _getPkmnDefHeaders(buffer);
}

int EngineWrapper::getMoveDefHeaders(std::vector<TextFileContentInfo>& buffer) const
{
    return _getMoveDefHeaders(buffer);
}

int EngineWrapper::init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs) const
{
    return _init(pkmnDefs, moveDefs);
}

int EngineWrapper::shutdown() const
{
    return _shutdown();
}

bool EngineWrapper::isReady() const
{
    return _isReady();
}
