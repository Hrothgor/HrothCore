#pragma once

namespace HrothCore
{
    class Shader
    {
        public:
            Shader(const std::string &vertexFile, const std::string &fragmentFile);
            ~Shader();

            void Start() const;
            void Stop() const;

            void Hotreload();

            uint32_t GetUniformLocation(const std::string &uniformName);

            void LoadFloat(uint32_t location, float value);
            void LoadBool(uint32_t location, bool value);
            void LoadVector2(uint32_t location, const glm::vec2 &value);
            void LoadVector3(uint32_t location, const glm::vec3 &value);
            void LoadMatrix(uint32_t location, const glm::mat4 &value);

            void Release();
        protected:
            uint32_t LoadShader(const std::string &file, uint32_t type) const;
            uint32_t LinkProgram(uint32_t vertexShaderID, uint32_t fragmentShaderID) const;

        private:
            uint32_t m_ProgramID;
            uint32_t m_VertexShaderID;
            uint32_t m_FragmentShaderID;

            std::string m_VertexFile;
            std::string m_FragmentFile;
    };
}