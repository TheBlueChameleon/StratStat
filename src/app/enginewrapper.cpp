#include <dlfcn.h>

#include <plog/Log.h>

#include <engine/interface.hpp>
#include "enginewrapper.hpp"

EngineWrapper::EngineWrapper(const std::filesystem::path& enginePath)
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

    PLOG_VERBOSE << "TESTING FOR SIGNATURE ...";
    const auto signaturePtr = findSymbol("getSignature");
    getSignature = reinterpret_cast<int(*)()>(signaturePtr);

    if (!getSignature)
    {
        PLOG_FATAL << "NO SIGNATURE DETECTED";
        std::exit(-1);
    }
    else if (getSignature() != EXPECTED_SIGNATURE)
    {
        PLOG_FATAL << "UNEXPECTED SIGNATURE";
        std::exit(-1);
    }
    PLOG_VERBOSE << "  ... SUCCESS!";
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
    PLOG_VERBOSE << "ATTEMPTING TO LOCATE " << symbolName << "...";

    char* error;
    auto* symbol = dlsym(handler, symbolName);
    if ((error = dlerror()) != nullptr && !symbol)
    {
        PLOG_ERROR << "COULD NOT FIND SYMBOL " << symbolName;
        PLOG_ERROR << error;
        return nullptr;
    }

    PLOG_VERBOSE << "  ... SUCCESS!";
    return symbol;
}
