#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace lw {

class Window;

class Application {
public:
    Application();
    ~Application();

    void run(const std::function<void()>& updateFunc);

    static void registerWindow(std::unique_ptr<Window>&& window);
    static void unregisterWindow(std::unique_ptr<Window>&& window);

private:
    void pollEvents();
    
    bool m_running = true;
    static std::map<uint32_t, std::unique_ptr<Window>> s_windows;

    // Prevent copying
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
};

} // namespace lw
