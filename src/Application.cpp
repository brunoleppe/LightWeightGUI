#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>


namespace lw
{
    class NativeWindow {
    public:
        ~NativeWindow();

        void onEvent(const SDL_Event& event);
        void swapBuffers();
        void clear(float r, float g, float b, float a);

        uint32_t getID() const { return m_id; }
        bool isOpen() const { return !m_shouldClose; }
        SDL_Window* getNativeWindow() const { return m_window; }
        SDL_GLContext getContext() const { return m_glContext; }

        static std::unique_ptr<NativeWindow> create(const std::string& title, int width, int height);

    private:

        NativeWindow(const std::string& title, int width, int height);

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;
        uint32_t m_id = 0;
        bool m_shouldClose = false;
    };

    std::unique_ptr<NativeWindow> NativeWindow::create(const std::string& title, int width, int height)
    {
        auto* window = new NativeWindow(title, width, height);
        return std::make_unique<NativeWindow>(*window);
    }

    NativeWindow::NativeWindow(const std::string& title, int width, int height)
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

    NativeWindow::~NativeWindow()
    {
        if (m_glContext) SDL_GL_DeleteContext(m_glContext);
        if (m_window) SDL_DestroyWindow(m_window);
    }

    void NativeWindow::onEvent(const SDL_Event& event)
    {
    }

    void NativeWindow::swapBuffers()
    {
        SDL_GL_SwapWindow(m_window);
    }

    void NativeWindow::clear(float r, float g, float b, float a)
    {
        SDL_GL_MakeCurrent(m_window, m_glContext);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

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

    uint32_t Application::CreateWindow(const std::string& title, int width, int height)
    {
        auto window = NativeWindow::create(title, width, height);
        int id = window->getID();
        registerWindow(std::move(window));
        return id;
    }

    void Application::registerWindow(std::unique_ptr<NativeWindow>&& window)
    {
        if (m_mainWindow == nullptr)
        {
            m_mainWindow = window.get();
        }
        m_windows[window->getID()] = std::move(window);
    }

    void Application::unregisterWindow(NativeWindow* window)
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

            std::cout << "window id " << e.window.windowID << std::endl;

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
