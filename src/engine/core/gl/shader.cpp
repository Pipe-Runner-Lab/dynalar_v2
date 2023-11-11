#include "shader.h"

Shader::Shader(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath) : m_shaderProgramID(0)
{
	const std::string vShaderSrc = ParseShader(vertexShaderFilePath);
	const std::string fShaderSrc = ParseShader(fragmentShaderFilePath);

	m_shaderProgramID = CreateShaderProgram(vShaderSrc, fShaderSrc);
}

Shader::~Shader()
{
	GL_CALL(glDeleteProgram(m_shaderProgramID));
}

GLuint Shader::CompileShader(const std::string &srcCode, GLenum shaderType)
{
	GL_CALL(GLuint shader = glCreateShader(shaderType));
	GLint codeLength = srcCode.length();
	const char *c_srcCode = srcCode.c_str();

	GL_CALL(glShaderSource(shader, 1, &c_srcCode, nullptr)); // notice address of pointer
	GL_CALL(glCompileShader(shader));

	// Check for errors
	GLint success = 0;
	GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLchar infoLog[1024] = {0};
		GL_CALL(glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog));

		fmt::print(stderr, "{}\n", infoLog);

		throw std::runtime_error(fmt::format(
				"Compilation Error: {} does not exist\n", (shaderType == GL_FRAGMENT_SHADER ? "Fragment Shader" : "Vertex Shader")));
		;
	}

	return shader;
}

GLuint Shader::CreateShaderProgram(const std::string &vShaderSrc, const std::string &fShaderSrc)
{
	GL_CALL(GLuint shaderProgramID = glCreateProgram());

	GLuint vertexShader = CompileShader(vShaderSrc, GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fShaderSrc, GL_FRAGMENT_SHADER);

	GL_CALL(glAttachShader(shaderProgramID, vertexShader));
	GL_CALL(glAttachShader(shaderProgramID, fragmentShader));

	GL_CALL(glLinkProgram(shaderProgramID)); // send program to GPU

	// Check for errors
	GLint success = 0;
	GL_CALL(glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLchar infoLog[1024] = {0};
		GL_CALL(glGetProgramInfoLog(shaderProgramID, sizeof(infoLog), NULL, infoLog));
		fmt::print(stderr, "{}\n", infoLog);

		throw std::runtime_error(fmt::format("Linking Error for Program ID {}\n", shaderProgramID));
	}

	// Validate program
	GL_CALL(glValidateProgram(shaderProgramID));
	GL_CALL(glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &success));
	if (!success)
	{
		GLchar infoLog[1024] = {0};
		GL_CALL(glGetProgramInfoLog(shaderProgramID, sizeof(infoLog), NULL, infoLog));
		fmt::print(stderr, "{}\n", infoLog);

		throw std::runtime_error(fmt::format("Validation Error for Program ID {}\n", shaderProgramID));
	}

	// Delete intermediate shader outputs
	GL_CALL(glDeleteShader(vertexShader));
	GL_CALL(glDeleteShader(fragmentShader));

	return shaderProgramID;
}

std::string Shader::ParseShader(const std::string &filename)
{
	std::string content = "";
	std::ifstream fileStream = std::ifstream(filename.c_str(), std::ios::in);

	if (!fileStream.is_open())
	{
		throw std::runtime_error(fmt::format("Failed to read shader file {}.\n", filename));
	}

	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::Bind()
{
	GL_CALL(glUseProgram(m_shaderProgramID));
}

void Shader::Unbind()
{
	GL_CALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string &name, int v0)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniform1i(location, v0));
}

void Shader::SetUniform1f(const std::string &name, float v0)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniform1f(location, v0));
}

void Shader::SetUniform3f(const std::string &name, float v0, float v1, float v2)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniform3f(location, v0, v1, v2));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4f(const std::string &name, const glm::mat4 &matrix)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::SetUniformBool(const std::string &name, bool value)
{
	GLint location = GetUniformLocation(name);
	if (location != -1)
		GL_CALL(glUniform1i(location, value));
}

GLint Shader::GetUniformLocation(const std::string &name)
{
	GL_CALL(GLint location = glGetUniformLocation(m_shaderProgramID, name.c_str()));

	if (location == -1)
	{
		fmt::print("Warning: {} does not exist\n", name);
	}

	return location;
}