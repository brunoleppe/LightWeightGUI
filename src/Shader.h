#pragma once
#include <glad/glad.h>
#include <string>

namespace lw
{
    class Shader
    {
    public:
        Shader(const char* vertexSource, const char* fragmentSource);
        ~Shader();

        void use() const;
        unsigned int getID() const { return m_id; }

    private:
        unsigned int m_id = 0;
        unsigned int compileShader(unsigned int type, const char* source);
    };
} // namespace lw
