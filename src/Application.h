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

    void run();

    void registerWindow(std::unique_ptr<Window>&& window);
    void unregisterWindow(Window* window);

private:
    void pollEvents();
    
    bool m_running = true;
    Window* m_mainWindow{nullptr};
    std::map<uint32_t, std::unique_ptr<Window>> m_windows;

    // Prevent copying
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
};

} // namespace lw
