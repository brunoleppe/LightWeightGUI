//
// Created by bruno on 6/5/26.
//

#include "Application.h"

#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
namespace lw
{
    Window::Window(const char* title, int width, int height, bool fullscreen)
    {
        if (!IsWindowReady())
        {
            InitWindow(width, height, title);
        }
    }

    Window::~Window()
    {
        CloseWindow();
    }

    void Window::Draw()
    {
        DrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY);

        // Circle shapes and lines
        DrawCircle(800 / 5, 120, 35, DARKBLUE);
        DrawCircleGradient(800 / 5.0f, 220.0f, 60, GREEN, SKYBLUE);
        DrawCircleLines(800 / 5, 340, 80, DARKBLUE);
        DrawEllipse(800 / 5, 120, 25, 20, YELLOW);
    }

#if defined(PLATFORM_WEB)
    Application* Application::s_app{nullptr};

    void Application::UpdateFrame()
    {
        if (s_app && s_app->m_window)
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            s_app->m_window->Draw();
            EndDrawing();
        }
    }
#endif

    Application::Application()
    {
#if defined(PLATFORM_WEB)
        s_app = this;
#endif
        SetConfigFlags(FLAG_MSAA_4X_HINT);
    }

    Application::~Application()
    {
        delete m_window;
        CloseWindow();
    }

    Window* Application::CreateWindow(const char* title, int width, int height, bool fullscreen)
    {
        m_window = new Window(title, width, height, fullscreen);
        return m_window;
    }

    void Application::Run()
    {
        if (m_window)
        {
#if defined(PLATFORM_WEB)
            // 0 = browser decides frame rate, true = simulate infinite loop
            emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
            while (!WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                m_window->Draw();
                EndDrawing();
            }
#endif
        }
    }
} // lw
