#ifndef ENGINE_HPP
#define ENGINE_HPP

class Engine
{
    private:
        Engine();

    public:
        static Engine& getInstance();

        Engine(Engine const&)           = delete;
        void operator=(Engine const&)   = delete;


};

#endif // ENGINE_HPP
