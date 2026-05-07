//
// Created by bruno on 6/5/26.
//

#ifndef LIGHTWEIGTHGUI_APPLICATION_H
#define LIGHTWEIGTHGUI_APPLICATION_H

namespace lw
{
    class Window
    {
    public:
        Window(const char* title, int width, int height, bool fullscreen = false);
        ~Window();
        void Draw();
    };

    class Application
    {
#if defined(PLATFORM_WEB)
        static Application* s_app;
        static void UpdateFrame();
#endif
        Window* m_window{nullptr};

    public:
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        Application();
        ~Application();
        Window* CreateWindow(const char* title, int width, int height, bool fullscreen = false);
        void Run();
    };
} // lw

#endif //LIGHTWEIGTHGUI_APPLICATION_H
