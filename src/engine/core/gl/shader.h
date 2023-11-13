#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "../utils/error.h"

class Shader {
private:
    GLuint m_shaderProgramID;
    std::string m_vertFilepath;
    std::string m_fragFilepath;

public:
    Shader(const std::string &vertexShaderFilePath,
           const std::string &fragmentShaderFilePath);
    ~Shader();

    void Bind();
    void Unbind();

    void SetUniform1i(const std::string &name, int v0);
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform3f(const std::string &name, float v0, float v1, float v2);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2,
                      float v3);
    void SetUniformMatrix4f(const std::string &name, const glm::mat4 &matrix);
    void SetUniformBool(const std::string &name, bool value);

private:
    GLuint CompileShader(const std::string &srcCode, GLenum shaderType);
    GLuint CreateShaderProgram(const std::string &vShaderSrc,
                               const std::string &fShaderSrc);
    std::string ParseShader(const std::string &filename);
    GLint GetUniformLocation(const std::string &name);
};
