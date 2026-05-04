#include "Application.h"
#include "Window.h"
#include "Shader.h"
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(int argc, char* argv[]) {
    lw::Application app;
    lw::Application::registerWindow(lw::Window::create("LightWeightGUI", 800, 600));
    lw::Application::registerWindow(lw::Window::create("LightWeightGUI2", 400, 400));
    lw::Shader shader(vertexShaderSource, fragmentShaderSource);


    app.run([&]() {
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);
        window1.clear(0.1f, 0.1f, 0.3f, 1.0f);
    });

    return 0;
}
