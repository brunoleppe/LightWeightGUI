#include "Application.h"
#include "Window.h"
#include <iostream>

namespace lw
{
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
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    }

    Application::~Application()
    {
        SDL_Quit();
    }

    void Application::run()
    {
        while (m_running && !m_windows.empty())
        {
            pollEvents();

            for (auto const& [id, window] : m_windows)
            {
                window->clear(0.2f, 0.2f, 0.2f, 1.0f);
                window->swapBuffers();
            }
        }
        m_windows.clear();

    }

    void Application::registerWindow(std::unique_ptr<Window>&& window)
    {
        if (m_mainWindow == nullptr)
        {
            m_mainWindow = window.get();
        }
        m_windows[window->getID()] = std::move(window);
    }

    void Application::unregisterWindow(Window* window)
    {
        m_windows.erase(window->getID());
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
                auto it = m_windows.find(e.window.windowID);
                if (it != m_windows.end())
                {
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        if (it->second.get() == m_mainWindow)
                        {
                            m_running = false;
                        }
                        unregisterWindow(it->second.get());

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
