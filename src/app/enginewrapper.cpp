#include <iostream>
#include <dlfcn.h>

#include <plog/Log.h>

#include <engine/interface.hpp>
#include "enginewrapper.hpp"

// TODO win version of this

void EngineWrapper::loadEninge(const std::filesystem::path& enginePath)
{
    PLOG_VERBOSE << "LOADING ENGINE FROM " << enginePath << "...";
    handler = dlopen(enginePath.c_str(), RTLD_LAZY);
    if (!handler)
    {
        PLOG_FATAL << "COULD NOT LOAD " << enginePath;
        PLOG_FATAL << dlerror();
        std::exit(-1);
    }
    PLOG_VERBOSE << "  ... SUCCESS!";
}

void EngineWrapper::testSignature()
{
    PLOG_VERBOSE << "TESTING FOR SIGNATURE ...";
    const auto signaturePtr = findSymbol("getSignature");
    _getSignature = reinterpret_cast<int(*)()>(signaturePtr);

    if (!_getSignature)
    {
        PLOG_FATAL << "NO SIGNATURE DETECTED";
        std::exit(-1);
    }
    else if (_getSignature() != EXPECTED_SIGNATURE)
    {
        PLOG_FATAL << "UNEXPECTED SIGNATURE";
        std::exit(-1);
    }
    PLOG_VERBOSE << "  ... SUCCESS!";
}

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
        PLOG_VERBOSE << "   ... EXTRACTED " << symbol;
    }

    this->*offset =target;
}

void EngineWrapper::extractFunctions()
{
    PLOG_VERBOSE << "EXTRACTING FUNCTIONS ...";

#define FETCH(symbol) fetchCheckAndTransfer(&EngineWrapper::_##symbol, #symbol)
    FETCH(getPkmnDefHeaders);
    FETCH(getMoveDefHeaders);
    FETCH(init);
    FETCH(shutdown);
    FETCH(isReady);
#undef FETCH

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
        dlclose(handler);
    }
}

void* EngineWrapper::findSymbol(const char* const symbolName)
{
    char* error = nullptr;
    auto* symbol = dlsym(handler, symbolName);
    if ((error = dlerror()) != nullptr && !symbol)
    {
        PLOG_ERROR << "COULD NOT FIND SYMBOL " << symbolName;
        PLOG_ERROR << error;
        return nullptr;
    }
    return symbol;
}

int EngineWrapper::getSignature() const
{
    return _getSignature();
}

int EngineWrapper::getPkmnDefHeaders(std::vector<std::string>& buffer) const
{
    return _getPkmnDefHeaders(buffer);
}

int EngineWrapper::getMoveDefHeaders(std::vector<std::string>& buffer) const
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
