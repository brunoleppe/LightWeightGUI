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
        BeginDrawing();
        ClearBackground(RAYWHITE);
        s_app->m_window->Draw();
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
            BeginDrawing();
            ClearBackground(RAYWHITE);
            m_window->GetRenderer()->Render({0,0,GetScreenWidth(),GetScreenHeight()});
            EndDrawing();
        }
#endif
    }
}
} // lw
