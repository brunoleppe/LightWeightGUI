#include "Application.h"
#include "Window.h"
#include <iostream>

namespace lw
{
    std::map<uint32_t, std::unique_ptr<Window>> Application::s_windows;

    Application::Application()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    }

    Application::~Application()
    {
        SDL_Quit();
    }

    void Application::run(const std::function<void()>& updateFunc)
    {
        while (m_running && !s_windows.empty())
        {
            pollEvents();

            if (updateFunc)
            {
                updateFunc();
            }

            for (auto const& [id, window] : s_windows)
            {
                window->swapBuffers();
            }
        }
    }

    void Application::registerWindow(std::unique_ptr<Window>&& window)
    {
        s_windows[window->getID()] = std::move(window);
    }

    void Application::unregisterWindow(std::unique_ptr<Window>&& window)
    {
        s_windows.erase(window->getID());
    }

    void Application::pollEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                m_running = false;
            }

            if (e.type == SDL_WINDOWEVENT)
            {
                auto it = s_windows.find(e.window.windowID);
                if (it != s_windows.end())
                {
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        unregisterWindow(std::move(it->second));
                    }
                    else
                    {
                        it->second->onEvent(e);
                    }
                }
            }

            // Handle other event types if necessary or forward to all windows if generic
        }
    }
} // namespace lw
