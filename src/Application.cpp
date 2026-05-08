//
// Created by bruno on 6/5/26.
//

#include "Application.h"

#include "raylib.h"
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
namespace lw {


#if defined(PLATFORM_WEB)
Application* Application::s_app{nullptr};

void Application::UpdateFrame() {
    if (s_app && s_app->m_window) {
        Vector2 mousePos = GetMousePosition();
        InputState input{
            static_cast<int>(mousePos.x), static_cast<int>(mousePos.y),
            IsMouseButtonDown(MOUSE_BUTTON_LEFT),
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
        };
        s_app->m_inputSystem.Process(s_app->m_window, input);

        BeginDrawing();
        s_app->m_window->GetRenderer()->Render({0, 0, GetScreenWidth(), GetScreenHeight()});
        EndDrawing();
    }
}
#endif

Application::Application() {
#if defined(PLATFORM_WEB)
    s_app = this;
#endif
    SetConfigFlags(FLAG_MSAA_4X_HINT);
}

Application::~Application() {
    delete m_window;
}

MainWindow* Application::CreateWindow(const char* title, int width, int height) {
    m_window = new MainWindow(title, width, height);
    return m_window;
}

void Application::Run() {
    if (m_window) {
#if defined(PLATFORM_WEB)
        // 0 = browser decides frame rate, true = simulate infinite loop
        emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
        while (!WindowShouldClose()) {
            Vector2 mousePos = GetMousePosition();
            InputState input{
                static_cast<int>(mousePos.x), static_cast<int>(mousePos.y),
                IsMouseButtonDown(MOUSE_BUTTON_LEFT),
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
            };
            m_inputSystem.Process(m_window, input);

            BeginDrawing();
            m_window->GetRenderer()->Render({0, 0, GetScreenWidth(), GetScreenHeight()});
            DrawText(TextFormat("Mouse X: %d\nMouse Y: %d", input.mouseX, input.mouseY), 10, 10, 20, BLACK);
            EndDrawing();
        }
#endif
    }
}
} // lw
