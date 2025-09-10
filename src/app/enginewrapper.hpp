#ifndef ENGINEWRAPPER_HPP
#define ENGINEWRAPPER_HPP

#include <filesystem>
#include <functional>

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

        int(*_getPkmnDefHeaders)(std::vector<std::string>&);
        int(*_getMoveDefHeaders)(std::vector<std::string>&);

        int(*_init)(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
        int(*_shutdown)();
        bool(*_isReady)();

        void loadEninge(const std::filesystem::__cxx11::path& enginePath);
        void testSignature();
        void extractFunctions();

        template<typename T>
        void fetchCheckAndTransfer(T EngineWrapper::* offset, const char* const symbol);

    public:
        EngineWrapper(const std::filesystem::path& enginePath);
        ~EngineWrapper();

        void* findSymbol(const char* const symbolName);

        int getSignature() const;
        int getPkmnDefHeaders(std::vector<std::string>& buffer) const;
        int getMoveDefHeaders(std::vector<std::string>& buffer) const;
        int init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs) const;
        int shutdown() const;
        bool isReady() const;

};

#endif // ENGINEWRAPPER_HPP
