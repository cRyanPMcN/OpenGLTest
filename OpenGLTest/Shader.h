#pragma once

#include <GLAD/gl.h>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>
#include <map>

class Shader {
public:
	unsigned int programId;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, glm::vec3& value) const;
	void SetVec4(const std::string& name, glm::quat& value) const;
	void SetVec4(const std::string& name, glm::vec4& value) const;
	void SetMat4(const std::string& name, glm::mat4& value) const;
};

struct ShaderUniform {
	const std::string name;
	const GLuint idProgram;
	const GLint idUniform;

	ShaderUniform(std::string nameString, GLuint programId, GLint uniformId) : name(nameString), idProgram(programId), idUniform(uniformId) {

	}

	ShaderUniform(const ShaderUniform& rhs) = default;

	ShaderUniform(ShaderUniform&& rhs) = default;

	ShaderUniform& operator=(const ShaderUniform& rhs) = default;

	ShaderUniform& operator=(ShaderUniform && rhs) = default;

	bool operator==(const ShaderUniform& rhs) {
		return (name == rhs.name && idProgram == rhs.idProgram && idUniform == rhs.idUniform);
	}

	bool operator!=(const ShaderUniform& rhs) {
		return !(operator==(rhs));
	}
};

struct GLShader {
	struct Config {	
		std::vector<std::pair<std::string, std::string>> replacementsVertex;
		std::vector<std::pair<std::string, std::string>> replacementsFragment;
		
		// File paths to shader stages
		std::vector<std::string> pathsVertex;
		std::vector<std::string> pathsFragment;

		std::string mainVertexShader;
		std::string mainFragmentShader;
	};
protected:
	GLuint programId;
	std::map<std::string, GLint> uniformCache;
public:
	GLShader(Config& config);

	GLShader(GLShader&&) = default;

	ShaderUniform SetBool(const std::string& name, bool value) {
		GLint uniformId = glGetUniformLocation(programId, name.c_str());
		glProgramUniform1i(programId, uniformId, value);
		return ShaderUniform(name, programId, uniformId);
	}

	bool SetBool(const ShaderUniform& uniform, bool value) {
		if (uniform.idProgram == programId) {
			glProgramUniform1i(programId, uniform.idUniform, value);
		}
		return glGetError() == GL_NO_ERROR;
	}

	//void SetBool(const std::string& name, bool value) {
	//	decltype(uniformCache)::const_iterator it = uniformCache.find(name);
	//	if (it == uniformCache.cend()) {
	//		GLint uniformId = glGetUniformLocation(programId, name.c_str());
	//		uniformCache.emplace(name, uniformId);
	//		glProgramUniform1i(programId, uniformId, value);
	//	}
	//	else {
	//		glProgramUniform1i(programId, it->second, value);
	//	}
	//}
};