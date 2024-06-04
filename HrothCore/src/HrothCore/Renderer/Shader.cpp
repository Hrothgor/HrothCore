#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Shader.hpp"

#include <glad/glad.h>
#include <fstream>

namespace HrothCore
{
    Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
        : m_VertexFile(vertexFile), m_FragmentFile(fragmentFile)
    {
        m_VertexShaderID = LoadShader(vertexFile, GL_VERTEX_SHADER);
        m_FragmentShaderID = LoadShader(fragmentFile, GL_FRAGMENT_SHADER);

        m_ProgramID = LinkProgram(m_VertexShaderID, m_FragmentShaderID);
    }

    Shader::~Shader()
    {
    }

    void Shader::Start() const
    {
        glUseProgram(m_ProgramID);
    }

    void Shader::Stop() const
    {
        glUseProgram(0);
    }

    void Shader::Hotreload()
    {
        Stop();
        glDeleteProgram(m_ProgramID);

        m_VertexShaderID = LoadShader(m_VertexFile, GL_VERTEX_SHADER);
        m_FragmentShaderID = LoadShader(m_FragmentFile, GL_FRAGMENT_SHADER);

        m_ProgramID = LinkProgram(m_VertexShaderID, m_FragmentShaderID);
    }

    uint32_t Shader::LoadShader(const std::string &file, unsigned int type) const
    {
        std::ifstream fs(file);

        if (!fs.is_open()) {
            HC_LOG_ERROR("Could not open shader file : {0}", file);
            return 0;
        }
        // Compile shader
        std::string source((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
        const GLchar *const src = source.c_str();
        GLuint shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &src, NULL);
        glCompileShader(shaderID);

        // Check for errors
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        GLchar infolog[512];
        if (!success) {
            glGetShaderInfoLog(shaderID, 512, NULL, infolog);
            HC_LOG_ERROR("Could not compile shader : {0}", file);
            HC_LOG_ERROR("\t{0}", infolog);
            glDeleteShader(shaderID);
            return 0;
        }
        return shaderID;
    }

    uint32_t Shader::LinkProgram(uint32_t vertexShaderID, uint32_t fragmentShaderID) const
    {
        if (vertexShaderID == 0 || fragmentShaderID == 0) {
            HC_LOG_ERROR("Can't link shader program, one of the shader is invalid");
            return 0;
        }
        // Validate program
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);
        glValidateProgram(programID);

        // Check for errors
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        GLchar infolog[512];
        if (!success) {
            glGetProgramInfoLog(programID, 512, NULL, infolog);
            HC_LOG_ERROR("Could not validate shader program: {0} {1}", m_VertexFile, m_FragmentFile);
            HC_LOG_ERROR("\t{0}", infolog);
            glDeleteProgram(programID);
            return 0;
        }

        // Clean up
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        HC_LOG_INFO("Shader program linked successfully : {0} {1}", m_VertexFile, m_FragmentFile);

        return programID;
    }

    uint32_t Shader::GetUniformLocation(const std::string &uniformName)
    {
        return glGetUniformLocation(m_ProgramID, uniformName.c_str());
    }

    void Shader::LoadFloat(uint32_t location, float value)
    {
        glUniform1f(location, value);
    }

    void Shader::LoadBool(uint32_t location, bool value)
    {
        glUniform1i(location, value);
    }

    void Shader::LoadVector2(uint32_t location, const glm::vec2 &value)
    {
        glUniform2f(location, value.x, value.y);
    }

    void Shader::LoadVector3(uint32_t location, const glm::vec3 &value)
    {
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::LoadMatrix(uint32_t location, const glm::mat4 &value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Release()
    {
        Stop();
        glDeleteProgram(m_ProgramID);
    }
}