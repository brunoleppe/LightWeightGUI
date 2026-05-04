#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <string>
#include <cstdint>
#include <memory>

namespace lw {

class Window {
public:
    ~Window();

    void onEvent(const SDL_Event& event);
    void swapBuffers();
    void clear(float r, float g, float b, float a);

    uint32_t getID() const { return m_id; }
    bool isOpen() const { return !m_shouldClose; }
    SDL_Window* getNativeWindow() const { return m_window; }
    SDL_GLContext getContext() const { return m_glContext; }

    static std::unique_ptr<Window> create(const std::string& title, int width, int height);

private:

    Window(const std::string& title, int width, int height);

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    uint32_t m_id = 0;
    bool m_shouldClose = false;
};

} // namespace lw
