#pragma once
#include <GLAD/gl.h>
#include <vector>
#include <string>
#include <cmath>
#include <stb_image.h>

enum TextureFormat {

};

class Texture2D : Object {
	GLuint textureId;
	GLsizei _width, _height;
	GLenum _internalFormat, _dataFormat;
public:
	Texture2D(int width, int height, int mipmapLevels = 0) : _width(width), _height(height), _internalFormat(GL_RGBA8), _dataFormat(GL_RGBA) {
		if (width <= 0 || height <= 0) {
			throw std::invalid_argument("Texture2D: Width and Height must be greater than zero.");
		}
		if (mipmapLevels <= 0) {
			int mipmaps = 0;
			int temp = std::max(width, height);
			while (temp != 1) {
				temp /= 2;
				++mipmaps;
			}
			mipmapLevels = mipmaps;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureStorage2D(textureId, mipmapLevels, _internalFormat, width, height);
		//glTextureSubImage2D(textureId, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, bytes);
		//glGenerateTextureMipmap(textureId);
	}

	Texture2D(const std::string& path) {
		int nrChannels;
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* imageData = stbi_load(path.c_str(), &_width, &_height, &nrChannels, 0);

		if (imageData == nullptr) {
			throw std::runtime_error("Texture2D(std::string& path): Failed to load image data for " + path + ".");
		}

		switch (nrChannels) {
		case 4:
			_internalFormat = GL_RGBA8;
			_dataFormat = GL_RGBA;
			break;
		case 3:
			_internalFormat = GL_RGB8;
			_dataFormat = GL_RGB;
			break;
		default:
			throw std::runtime_error("Texture2D(std::string & path): Unexpected number of channels for " + path + " received " + std::to_string(nrChannels) + ".");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int mipmaps = 0;
		int temp = std::max(_width, _height);
		while (temp != 1) {
			temp /= 2;
			++mipmaps;
		}

		glTextureStorage2D(textureId, mipmaps, _internalFormat, _width, _height);

		glTextureSubImage2D(textureId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(textureId);

		stbi_image_free(imageData);
	}

	~Texture2D() {
		glDeleteTextures(1, &textureId);
	}

	bool SetData(std::vector<char> bytes) {
		if (bytes.size() != size_t(_width * _height * Channels())) {
		
		}
		glTextureSubImage2D(textureId, 0, 0, 0, _width, _height, _dataFormat, GL_BYTE, bytes.data());
	}

	bool SetData(std::vector<unsigned char> bytes) {
		if (bytes.size() != size_t(_width * _height * Channels())) {
		
		}
		glTextureSubImage2D(textureId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, bytes.data());
	}

	int Channels() {
		switch (_dataFormat) {
		case GL_RGBA:
			return 4;
		case GL_RGB:
			return 3;
		}
	}
};