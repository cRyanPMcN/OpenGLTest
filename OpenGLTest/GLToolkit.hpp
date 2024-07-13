#pragma once
#include "Object.hpp"
#include "GLTF.hpp"
#include <GLAD/gl.h>
#include <algorithm>
#include <string>

#define FILE_FUNCTION_LINE std::string(__FILE__) + ':' + std::string(__FUNCTION__) + '@' + std::to_string(__LINE__)

namespace Math {

}

struct Vec2I {
	long long x, y;

	Vec2I() : x(0), y(0) {

	}

	Vec2I(decltype(x) _x, decltype(y) _y) : x(_x), y(_y) {

	}

	Vec2I(Vec2I const&) = default;
	Vec2I(Vec2I&&) = default;

	Vec2I& operator=(Vec2I const&) = default;
	Vec2I& operator=(Vec2I&&) = default;
};

struct Vec3I {
	long long x, y, z;

	Vec3I() : x(0), y(0), z(0) {

	}

	Vec3I(decltype(x) _x, decltype(y) _y, decltype(z) _z) : x(_x), y(_y), z(_z) {

	}

	Vec3I(Vec3I const&) = default;
	Vec3I(Vec3I&&) = default;

	Vec3I& operator=(Vec3I const&) = default;
	Vec3I& operator=(Vec3I&&) = default;

	operator Vec2I() {
		return Vec2I(x, y);
	}
};

struct Vec2 {
	float x, y;

	Vec2() : x(0), y(0) {

	}

	Vec2(decltype(x) _x, decltype(y) _y) : x(_x), y(_y) {

	}

	Vec2(Vec2 const&) = default;
	Vec2(Vec2&&) = default;

	Vec2& operator=(Vec2 const&) = default;
	Vec2& operator=(Vec2&&) = default;
};

struct Vec3 {
	float x, y, z;

	Vec3() : x(0), y(0), z(0) {

	}

	Vec3(decltype(x) _x, decltype(y) _y, decltype(z) _z) : x(_x), y(_y), z(_z) {

	}

	Vec3(Vec3 const&) = default;
	Vec3(Vec3&&) = default;

	Vec3& operator=(Vec3 const&) = default;
	Vec3& operator=(Vec3&&) = default;

	operator Vec2() {
		return Vec2(x, y);
	}
};

struct Vec4 {
	float x, y, z, w;

	Vec4() : x(0.0), y(0.0), z(0.0), w(0.0) {

	}

	Vec4(decltype(x) _x, decltype(y) _y, decltype(z) _z, decltype(w) _w) : x(_x), y(_y), z(_z), w(_w) {

	}

	Vec4(Vec4 const&) = default;
	Vec4(Vec4&&) = default;

	Vec4& operator=(Vec4 const&) = default;
	Vec4& operator=(Vec4&&) = default;

	operator Vec3() {
		return Vec3(x, y, z);
	}

	float& operator[](size_t index) {
		switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			throw std::runtime_error("Vec4::operator[](size_t): index must be 0, 1, 2, or 3.");
		}
	}
};

struct Mat4x4 {
	Vec4 data[4];

	Mat4x4() = default;

	// Generate as Identity
	Mat4x4(double value) {
		data[0][0] = (float)value;
		data[1][1] = (float)value;
		data[2][2] = (float)value;
		data[3][3] = (float)value;
	}

	Vec4& operator[](size_t index) {
		return data[index];
	}
};

static Mat4x4 GL_Camera_To_Matrix(GLTF::Camera const& camera) {
	Mat4x4 data;
	if (camera.type == GLTF::Constants::PERSPECTIVE) {
		if (camera.perspective.zfar == std::numeric_limits<decltype(camera.perspective.zfar)>::infinity()) {

		}
		else {
			double tanHalfFovy = tan(0.5 * camera.perspective.yfov);
			data[0][0] = (float)(1.0 / (camera.perspective.aspectRatio * tanHalfFovy));
			data[1][1] = (float)(1.0 / tanHalfFovy);
			data[2][2] = (float)((camera.perspective.zfar + camera.perspective.znear) / (camera.perspective.znear - camera.perspective.zfar));
			data[2][3] = (float)(-1.0);
			data[3][2] = (float)((2.0 * camera.perspective.zfar * camera.perspective.znear) / (camera.perspective.znear - camera.perspective.zfar));
		}
	}
	else if (camera.type == GLTF::Constants::ORTHOGRAPHIC) {
		data[0][0] = float(2.0 / camera.orthographic.xmag);
		data[1][1] = float(2.0 / camera.orthographic.ymag);
		data[2][2] = float(2.0 / (camera.orthographic.znear - camera.orthographic.zfar));
		data[3][2] = float((camera.orthographic.zfar + camera.orthographic.znear) / (camera.orthographic.znear - camera.orthographic.zfar));
		data[3][3] = float(1);
	}
	else {
		throw std::runtime_error(FILE_FUNCTION_LINE + ": Unknowqn camera.type.");
	}
	return data;
}

struct GLCamera : public Object {
	enum Type {
		None,
		Orthographic,
		Perspective
	} type;

	union Data {
		struct Orthographic {
			double xmag;
			double ymag;
			double zfar;
			double znear;
		} orthographic;
		struct Perspective {
			double aspectRatio;
			double yfov;
			double zfar;
			double znear;
		} perspective;
	} cameraData;
	Mat4x4 data;

	GLCamera(GLTF::Camera const& camera) : type(Type::None), Object(camera.name) {
		glm::identity<glm::mat4>();
		if (camera.type == GLTF::Constants::PERSPECTIVE) {
			type = Type::Perspective;
			if (camera.perspective.zfar == std::numeric_limits<decltype(camera.perspective.zfar)>::infinity()) {

			}
			else {
				double tanHalfFovy = tan(0.5 * camera.perspective.yfov);
				data[0][0] = (float)(1.0 / (camera.perspective.aspectRatio * tanHalfFovy));
				data[1][1] = (float)(1.0 / tanHalfFovy);
				data[2][2] = (float)((camera.perspective.zfar + camera.perspective.znear) / (camera.perspective.znear - camera.perspective.zfar));
				data[2][3] = (float)(-1.0);
				data[3][2] = (float)((2.0 * camera.perspective.zfar * camera.perspective.znear) / (camera.perspective.znear - camera.perspective.zfar));
			}
		}
		else if (camera.type == GLTF::Constants::ORTHOGRAPHIC) {
			type = Type::Orthographic;
			//data[0][0] = 2.0 / (right - left);
			//data[1][1] = 2.0 / (top - bottom);
			//data[2][2] = -2.0 / (camera.orthographic.zfar - camera.orthographic.znear);
			//data[3][0] = -(right + left) / (right - left);
			//data[3][1] = -(top + bottom) / (top - bottom);
			//data[3][2] = -(camera.orthographic.zfar + camera.orthographic.znear) / (camera.orthographic.zfar - camera.orthographic.znear);

			data[0][0] = float(double(0.5) / camera.orthographic.xmag);
			data[1][1] = float(double(0.5) / camera.orthographic.ymag);
			data[2][2] = float(2.0 / (camera.orthographic.znear - camera.orthographic.zfar));
			data[3][2] = float((camera.orthographic.zfar + camera.orthographic.znear) / (camera.orthographic.znear - camera.orthographic.zfar));
			data[3][3] = float(1);
		}
	}
};

/// <summary>
/// Utility class for decoding a base64 data-stream as well as parsing the mimeType
/// </summary>
struct DataStreamBase64 {
	std::string mimeType;
	std::vector<unsigned char> binaryData;

	static std::vector<unsigned char> Base64_Decode(std::string const& source) {
		static const unsigned char INDEX_ARRAY[256] = {
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
			0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
			0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};
		if (source.size() == 0) {
			return std::vector<unsigned char>();
		}
		if (source.size() % 4 != 0) {
			throw std::runtime_error(std::string(__FILE__) + ':' + std::string(__FUNCTION__) + '@' + std::to_string(__LINE__) + ": Base64 Stream length must be a multiple of four.");
		}

		const unsigned short paddingCount = source[source.size() - 1] == '=' + source[source.size() - 2] == '=';
		const size_t FINALUNPADDED = source.size() - (4 * size_t(paddingCount != 0));
		std::vector<unsigned char> result((source.size() / 4 * 3) - paddingCount, unsigned char());

		size_t idxResult = 0;
		int number = 0;
		unsigned char const* sourcePointer = (unsigned char const*)source.data();
		unsigned char* resultPointer = result.data();
		for (size_t idxSource = 0; idxSource != FINALUNPADDED; idxSource += 4) {
			number = INDEX_ARRAY[sourcePointer[idxSource]] << 18
				| INDEX_ARRAY[sourcePointer[idxSource + 1]] << 12
				| INDEX_ARRAY[sourcePointer[idxSource + 2]] << 6
				| INDEX_ARRAY[sourcePointer[idxSource + 3]];

			resultPointer[idxResult] = number >> 16;
			resultPointer[idxResult + 1] = number >> 8;
			resultPointer[idxResult + 2] = number;
			idxResult += 3;
		}

		switch (paddingCount) {
		case 2:
			number = INDEX_ARRAY[sourcePointer[FINALUNPADDED]] << 18 | INDEX_ARRAY[sourcePointer[FINALUNPADDED + 1]] << 12;
			resultPointer[idxResult] = static_cast<char>(number >> 16);
			break;
		case 1:
			number = INDEX_ARRAY[sourcePointer[FINALUNPADDED]] << 18 | INDEX_ARRAY[sourcePointer[FINALUNPADDED + 1]] << 12 | INDEX_ARRAY[sourcePointer[FINALUNPADDED + 2]] << 6;
			resultPointer[idxResult] = static_cast<char>(number >> 16);
			resultPointer[idxResult + 1] = static_cast<char>(number >> 8);
			break;
		case 0:
			break;
		};

		return result;
	}

	// dataStream should be a std::vector<unsigned char>, input to this constructor is GLTF::Buffer::uri which is a string
	DataStreamBase64(std::string const& dataStream) : 
		mimeType(dataStream.substr(dataStream.find(GLTF::Constants::STREAM_DATA) + GLTF::Constants::STREAM_DATA.length(), dataStream.find(GLTF::Constants::STREAM_SEPERATOR))), 
		binaryData(Base64_Decode(dataStream.substr(dataStream.find(GLTF::Constants::STREAM_SEPERATOR) + GLTF::Constants::STREAM_SEPERATOR.length()))) {
	}
};

/// <summary>
/// Loads data from a GLTF::Buffer object
/// </summary>
struct GLBuffer : public Object {
	std::vector<unsigned char> bufferData;

	GLBuffer(GLTF::Buffer const& buffer) : bufferData(Load_Data(buffer)) {

	}

	static std::vector<unsigned char> Load_Data_From_File(std::filesystem::path const& pathToFile) {
		// Load a binary file and push directly to the bufferStorage
		std::string data;
		{
			std::ifstream file(pathToFile, std::ifstream::binary);
			if (file.is_open()) {
				std::stringstream stringSource;
				stringSource << file.rdbuf();
				data = stringSource.str();
				file.close();
			}
			else {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": failed to open file '" + pathToFile.string() + "'.");
			}
		}

		return std::vector<unsigned char>(data.cbegin(), data.cend());
	}

	static std::vector<unsigned char> Load_Data(GLTF::Buffer const& buffer) {
		if (buffer.uri.find(GLTF::Constants::STREAM_DATA) == 0) {
			DataStreamBase64 data(buffer.uri);
			 
			if (data.binaryData.size() != buffer.byteLength) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": buffer data not of expected byteLength:" + std::to_string(buffer.byteLength) + ".");
			}

			return data.binaryData;
		}
		else {
			// Assume URI is a file-path
			std::filesystem::path bufferPath(buffer.uri);
			if (std::filesystem::is_directory(buffer.uri)) {
				// exception, buffer.uri is not a file
			}

			if (bufferPath.extension() != ".bin") {
				// exception, buffer.uri is not a binary file
			}
			
			// For relative paths the std::filesystem::current_path is expected 
			//    to be set to the directory of the GLTF file that is being parsed
			std::vector<unsigned char> const data = Load_Data_From_File(bufferPath);

			if (data.size() != buffer.byteLength) {
				// exception, loaded data is not of expected size
			}
			return data;
		}
	}
};

class GLTexture;

class GLSampler : public Object {
	GLuint _idSampler;
public:
	GLSampler(GLTF::Sampler const& sampler) : _idSampler(0), Object(sampler.name) {
		glCreateSamplers(1, &_idSampler);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_S, (GLint)sampler.wrapS);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_T, (GLint)sampler.wrapT);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MAG_FILTER, (GLint)sampler.magFilter);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MIN_FILTER, (GLint)sampler.minFilter);
	}

	GLSampler(GLSampler const& rhs) : _idSampler(0), Object(rhs.name) {
		glCreateSamplers(1, &_idSampler);
		GLint valueToCopy[4];
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_WRAP_S, &valueToCopy[0]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_WRAP_T, &valueToCopy[1]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_MAG_FILTER, &valueToCopy[2]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_MIN_FILTER, &valueToCopy[3]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_S, valueToCopy[0]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_T, valueToCopy[1]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MAG_FILTER, valueToCopy[2]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MIN_FILTER, valueToCopy[3]);
	}

	GLSampler(GLSampler&&) = default;

	~GLSampler() {
		glDeleteSamplers(1, &_idSampler);
	}

	GLSampler& operator=(GLSampler const& rhs) {
		glCreateSamplers(1, &_idSampler);
		GLint valueToCopy[4];
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_WRAP_S, &valueToCopy[0]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_WRAP_T, &valueToCopy[1]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_MAG_FILTER, &valueToCopy[2]);
		glGetSamplerParameteriv(rhs._idSampler, GL_TEXTURE_MIN_FILTER, &valueToCopy[3]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_S, valueToCopy[0]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_WRAP_T, valueToCopy[1]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MAG_FILTER, valueToCopy[2]);
		glSamplerParameteri(_idSampler, GL_TEXTURE_MIN_FILTER, valueToCopy[3]);
	}

	GLSampler& operator=(GLSampler&&) = default;

	friend class GLTexture;
};
 
class GLImage {
	static GLImage errorImage;
	void* data;

	int width, height, channels;
public:
	GLImage() : data(nullptr), width(-1), height(-1), channels(-1) {

	}

	GLImage(GLTF::Image const& image) : data(nullptr), width(-1), height(-1), channels(-1) {
		if (!image.uri.empty()) {
			if (image.uri.find(GLTF::Constants::STREAM_DATA) == 0) {
				DataStreamBase64 base64(image.uri);

				if (image.mimeType == GLTF::Constants::MIME_IMAGE_PNG) {

				}
				else if (image.mimeType == GLTF::Constants::MIME_IMAGE_JPEG) {
					stbi_loadf_from_memory(base64.binaryData.data(), (int)base64.binaryData.size(), &width, &height, &channels, 0);
				}
				
			}
			else {
				if (image.mimeType.empty()) {
					// Exception, no mimeType needed for load from buffer
				}
				if (image.bufferView != -1) {
					// Load from buffer
				}

			}
			// Try to load
			std::filesystem::path filePath(image.uri);

		}
	}

	GLImage(GLTF::Image&& image) : data(nullptr), width(-1), height(-1), channels(-1) {
		if (!image.uri.empty()) {
			if (image.uri.find(GLTF::Constants::STREAM_DATA) == 0) {
				DataStreamBase64 base64(image.uri);

				if (image.mimeType == GLTF::Constants::MIME_IMAGE_PNG) {

				}
				else if (image.mimeType == GLTF::Constants::MIME_IMAGE_JPEG) {
					stbi_loadf_from_memory(base64.binaryData.data(), base64.binaryData.size(), &width, &height, &channels, 0);
				}
			}
			// Try to load
			std::filesystem::path filePath;
		}
		else {
			if (image.bufferView != -1) {

			}
		}
	}

	~GLImage() {
		delete data;
	}
};

class GLTexture {
	GLuint _idTexture;
public:
	GLTexture(GLTF::Texture const& texture, GLSampler const& sampler) {
		glCreateTextures(GL_TEXTURE_2D, 1, &_idTexture);
		//glTextureBuffer()
	}
};

struct GLNode {

};

struct GLObject : public Object {
	std::vector<GLNode> nodes;
	std::vector<GLNode> rootNodes;

};
