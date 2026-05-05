#pragma once
#include <map>
#include <memory>
#include <SDL2/SDL.h>

namespace lw {

class NativeWindow;

class Application {
public:
    Application();
    ~Application();

    void run();


    uint32_t CreateWindow(const std::string& title, int width, int height);

private:
    void pollEvents();

    void registerWindow(std::unique_ptr<NativeWindow>&& window);
    void unregisterWindow(NativeWindow* window);

    bool m_running = true;
    NativeWindow* m_mainWindow{nullptr};
    std::map<uint32_t, std::unique_ptr<NativeWindow>> m_windows;

    // Prevent copying
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
};

} // namespace lw
