#include "Shader.h"
#include <iostream>
#include <vector>

namespace lw
{
    Shader::Shader(const char* vertexSource, const char* fragmentSource)
    {
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        m_id = glCreateProgram();
        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);

        int success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_id);
    }

    void Shader::use() const
    {
        glUseProgram(m_id);
    }

    unsigned int Shader::compileShader(unsigned int type, const char* source)
    {
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        int success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        return id;
    }
} // namespace lw
