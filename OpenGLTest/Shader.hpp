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
	struct ShaderResource {
		GLuint index;
		GLuint type;

		ShaderResource(GLuint i, GLuint t) : index(i), type(t) {

		}

		ShaderResource(ShaderResource const&) = default;
		ShaderResource(ShaderResource&&) = default;

		ShaderResource& operator=(ShaderResource const&) = default;
		ShaderResource& operator=(ShaderResource&&) = default;
	};
	using ResourceMap = std::map<std::string, ShaderResource>;
protected:
	GLuint programId;
	ResourceMap inputAttributes;
	ResourceMap uniformCache;
	ResourceMap uniformBlocks;
	ResourceMap outputAttributes;
public:
	GLShader(Config& config);

	GLShader(GLShader&&) = default;

	bool SetValue(const std::string& name, bool value) {
		ResourceMap::const_iterator result(uniformCache.find(name));
		if (result != uniformCache.cend()) {
			glProgramUniform1i(programId, result->second.index, value);
			return true;
		}
		else {
			return false;
		}
	}

	ResourceMap const& Inputs() const {
		return inputAttributes;
	}
	
	ResourceMap const& Uniforms() const {
		return uniformCache;
	}
	
	ResourceMap const& UniformBlocks() const {
		return uniformBlocks;
	}
	
	ResourceMap const& Ouputs() const {
		return outputAttributes;
	}
};