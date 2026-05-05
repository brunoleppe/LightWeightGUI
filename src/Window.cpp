#include "Window.h"
#include "Application.h"
#include <iostream>

namespace lw
{
    std::unique_ptr<Window> Window::create(const std::string& title, int width, int height)
    {
        auto* window = new Window(title, width, height);
        return std::make_unique<Window>(*window);
    }

    Window::Window(const std::string& title, int width, int height)
    {
        m_window = SDL_CreateWindow(title.c_str(),
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (!m_window)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        m_id = SDL_GetWindowID(m_window);

        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext)
        {
            std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (SDL_GL_MakeCurrent(m_window, m_glContext) < 0)
        {
            std::cerr << "OpenGL context could not be made current! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        static bool gladInitialized = false;
        if (!gladInitialized)
        {
            if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
            {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                return;
            }

            gladInitialized = true;
        }
    }

    Window::~Window()
    {
        if (m_glContext) SDL_GL_DeleteContext(m_glContext);
        if (m_window) SDL_DestroyWindow(m_window);
    }

    void Window::onEvent(const SDL_Event& event)
    {
    }

    void Window::swapBuffers()
    {
        SDL_GL_SwapWindow(m_window);
    }

    void Window::clear(float r, float g, float b, float a)
    {
        SDL_GL_MakeCurrent(m_window, m_glContext);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
} // namespace lw
