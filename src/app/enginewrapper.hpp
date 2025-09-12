#ifndef ENGINEWRAPPER_HPP
#define ENGINEWRAPPER_HPP

#include <filesystem>
#include <functional>

#include <engine/interface.hpp>

#ifdef _WIN32
    #include <windows.h>
#endif

class EngineWrapper
{
    private:
#ifdef _WIN32
        HINSTANCE handler;
#else
        void* handler = nullptr;
#endif
        int(*_getSignature)();

        void(*_getPkmnDefHeaders)(std::vector<VariantContentInfo>&);
        void(*_getMoveDefHeaders)(std::vector<VariantContentInfo>&);

        void(*_init)(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
        void(*_connectLogger)(const std::shared_ptr<spdlog::logger>& logger);
        void(*_shutdown)();
        bool(*_isReady)();

        void loadEninge(const std::filesystem::path& enginePath);
        void testSignature();
        void extractFunctions();

        template<typename T>
        void fetchCheckAndTransfer(T EngineWrapper::* offset, const char* const symbol);

    public:
        EngineWrapper(const std::filesystem::path& enginePath);
        ~EngineWrapper();

        void* findSymbol(const char* const symbolName);

        int  getSignature() const;
        void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer) const;
        void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer) const;
        void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs) const;
        void connectLogger(const std::shared_ptr<spdlog::logger>& logger) const;
        void shutdown() const;
        bool isReady() const;
};

#endif // ENGINEWRAPPER_HPP
