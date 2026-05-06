#include "Application.h"
#include "Shader.h"
#include <iostream>

#include "GUI/Delegate.h"
#include "GUI/Widget.h"

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
    app.CreateWindow("LightWeightGUI", 800, 600);
    app.CreateWindow("LightWeightGUI2", 400, 400);
    lw::Shader shader(vertexShaderSource, fragmentShaderSource);

    app.run();

    return 0;
}
