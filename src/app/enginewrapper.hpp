#ifndef ENGINEWRAPPER_HPP
#define ENGINEWRAPPER_HPP

#include <filesystem>
#include <functional>

class EngineWrapper
{
    private:
        void* handler = nullptr;

        std::function<int()> getSignature;

    public:
        EngineWrapper(const std::filesystem::path& enginePath);
        ~EngineWrapper();

        void* findSymbol(const char* const symbolName);
};

#endif // ENGINEWRAPPER_HPP
