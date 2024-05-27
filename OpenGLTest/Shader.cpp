#include "Shader.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

unsigned int Compile_Shader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)_malloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cerr << "Failed to " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " compile shader!" << std::endl;
		std::cerr << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Create_Shader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = Compile_Shader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = Compile_Shader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		//char* message = (char*)_malloca(length * sizeof(char));
		char* message = (char*)malloc(length * sizeof(char));

		glGetProgramInfoLog(program, length, &length, message);

		std::cerr << "Failed to link shader!" << std::endl;
		std::cerr << message << std::endl;

		glDeleteShader(program);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

try {
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	vShaderFile.close();
	fShaderFile.close();
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
}
catch (std::ifstream::failure e) {
	std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
}

programId = Create_Shader(vertexCode, fragmentCode);
}

Shader::~Shader() {
	glDeleteShader(programId);
}

void Shader::Use() {
	glUseProgram(programId);
}

void Shader::SetBool(const std::string& name, bool value) const {
	glProgramUniform1i(programId, glGetUniformLocation(programId, name.c_str()), value);
	//glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value) const {
	glProgramUniform1i(programId, glGetUniformLocation(programId, name.c_str()), value);
	//glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
	glProgramUniform1f(programId, glGetUniformLocation(programId, name.c_str()), value);
	//glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3& value) const {
	glProgramUniform3fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	//glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetVec4(const std::string& name, glm::quat& value) const {
	glProgramUniform4fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	//glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetVec4(const std::string& name, glm::vec4& value) const {
	glProgramUniform4fv(programId, glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
	//glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4& value) const {
	glProgramUniformMatrix4fv(programId, glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
	//glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

static inline std::string Read_File(std::filesystem::path const& path, std::vector<std::pair<std::string, std::string>> const& replacements) {
	std::ifstream shaderFile(path);
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::ostringstream shaderSource;
	shaderSource << shaderFile.rdbuf();
	shaderFile.close();
	std::string shaderCode = shaderSource.str();

	for (std::pair<std::string, std::string> const& replace : replacements) {
		for (size_t idx = shaderCode.find(replace.first); idx != std::string::npos; idx = shaderCode.find(replace.first, idx + replace.second.size())) {
			shaderCode.replace(idx, replace.first.size(), replace.second);
		}
	}

	return shaderCode;
}

static inline std::string Read_Paths(std::vector<std::string> const& paths, std::vector<std::pair<std::string, std::string>> const& replacements) {
	std::string concatenatedCode;

	for (std::string const& pathString : paths) {
		std::string shaderCode;
		std::filesystem::path filePath(pathString);
		if (std::filesystem::is_empty(filePath)) {
			// Path points to an empty file/directoy so skip it
			continue;
		}
		if (std::filesystem::is_directory(filePath)) {
			for (std::filesystem::directory_iterator iterDir = std::filesystem::directory_iterator(filePath); iterDir != std::filesystem::end(iterDir); ++iterDir) {
				concatenatedCode += Read_File(filePath, replacements);
			}
		}
		else {
			concatenatedCode += Read_File(filePath, replacements);
		}
	}

	return concatenatedCode;
}

GLShader::GLShader(Config& config) : programId(glCreateProgram()) {
	std::string vertexCode = Read_Paths(config.pathsVertex, config.replacementsVertex);
	vertexCode += config.mainVertexShader;

	std::string fragmentCode = Read_Paths(config.pathsFragment, config.replacementsFragment);
	fragmentCode += config.mainFragmentShader;

	programId = Create_Shader(vertexCode, fragmentCode);
}