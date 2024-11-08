#include "Shader.hpp"
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

		std::string message(length, char(0));

		glGetShaderInfoLog(id, length, &length, message.data());

		glDeleteShader(id);
		if (type == GL_VERTEX_SHADER) {
			throw std::exception(("Failed to compile vertex shader: " + message).data());
		}
		else if (type == GL_VERTEX_SHADER) {
			throw std::exception(("Failed to compile fragment shader: " + message).data());
		}
		else {
			throw std::exception(("Failed to compile unknown shader: " + message).data());
		}
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

		std::string message(length, char(0));

		glGetProgramInfoLog(program, length, &length, message.data());

		throw std::exception(("Failed to link shader: " + message).data());

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
	std::string shaderCode;
	{
		std::ifstream shaderFile(path);
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::ostringstream shaderSource;
		shaderSource << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderSource.str();
	}

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
			// Path points to an empty file/directory so skip it
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

typename GLShader::ResourceMap Get_Program_Interface_Name_Index(GLuint programId, GLenum interfaceId) {
	const GLenum INFOQUERY[2] = { GL_NAME_LENGTH, GL_TYPE };
	GLint activeResourceCount;
	GLShader::ResourceMap nameIndexMap;
	glGetProgramInterfaceiv(programId, interfaceId, GL_ACTIVE_RESOURCES, &activeResourceCount);

	for (decltype(activeResourceCount) attributeIndex = 0; attributeIndex < activeResourceCount; ++attributeIndex) {
		GLint resultInfo[2];
		glGetProgramResourceiv(programId, interfaceId, attributeIndex, 1, INFOQUERY, 1, NULL, resultInfo);

		std::string nameBuffer(resultInfo[0], char(0));
		glGetProgramResourceName(programId, interfaceId, attributeIndex, nameBuffer.length(), NULL, nameBuffer.data());
		nameIndexMap.emplace(nameBuffer, GLShader::ShaderResource(attributeIndex, resultInfo[1]));
	}

	return nameIndexMap;
}

GLShader::GLShader(Config& config) {
	std::string vertexCode = Read_Paths(config.pathsVertex, config.replacementsVertex).append(config.mainVertexShader);

	std::string fragmentCode = Read_Paths(config.pathsFragment, config.replacementsFragment).append(config.mainFragmentShader);

	programId = Create_Shader(vertexCode, fragmentCode);

	inputAttributes = Get_Program_Interface_Name_Index(programId, GL_PROGRAM_INPUT);
	uniformCache = Get_Program_Interface_Name_Index(programId, GL_UNIFORM);
	uniformBlocks = Get_Program_Interface_Name_Index(programId, GL_UNIFORM_BLOCK);
	outputAttributes = Get_Program_Interface_Name_Index(programId, GL_PROGRAM_OUTPUT);
}