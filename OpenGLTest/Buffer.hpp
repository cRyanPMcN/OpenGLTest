#pragma once
#include "Object.hpp"
#include "BufferFormat.hpp"
#include <GLAD\gl.h>
#include <vector>

struct VertexArray;

/// <summary
/// Pass to buffers to specify that size may be changed after creation
/// </summary>
struct MutableBufferT {};

GLuint Create_Single_Buffer() {
	GLuint _bufferId;
	glCreateBuffers(1, &_bufferId);
	return _bufferId;
}

struct Buffer : public Object {
protected:
	const GLuint _bufferId;
	GLsizeiptr _bufferSize;
	const bool _isResizable;
public:
	Buffer(GLsizeiptr bufferSize) : _bufferId(Create_Single_Buffer()), _bufferSize(bufferSize), _isResizable(false) {
		glNamedBufferStorage(_bufferId, bufferSize, 0, GL_DYNAMIC_STORAGE_BIT);
	}

	Buffer(GLsizeiptr bufferSize, MutableBufferT) : _bufferId(Create_Single_Buffer()), _bufferSize(bufferSize), _isResizable(true) {
		glNamedBufferData(_bufferId, bufferSize, 0, GL_STATIC_DRAW);
	}

	Buffer(GLsizeiptr bufferSize, void* data) : _bufferId(Create_Single_Buffer()), _bufferSize(bufferSize), _isResizable(false) {
		glNamedBufferStorage(_bufferId, bufferSize, data, GL_DYNAMIC_STORAGE_BIT);
	}

	Buffer(GLsizeiptr bufferSize, void* data, MutableBufferT) : _bufferId(Create_Single_Buffer()), _bufferSize(bufferSize), _isResizable(true) {
		glNamedBufferData(_bufferId, bufferSize, data, GL_STATIC_DRAW);
	}

	virtual ~Buffer() {
		glDeleteBuffers(1, &_bufferId);
	}

	GLuint BufferId() {
		return _bufferId;
	}

	GLsizeiptr Size() {
		return _bufferSize;
	}

	bool IsResizable() {
		return _isResizable;
	}
	
	virtual bool SwapBufferData(void* data, GLsizeiptr dataSize, GLintptr offset = 0) {
		if (!_isResizable && (dataSize + offset) > _bufferSize) {
			return false;
		}

		glNamedBufferSubData(_bufferId, offset, _bufferSize, 0);
		glNamedBufferSubData(_bufferId, offset, dataSize, data);
		return true;
	}

	template <class _Ty>
	bool SwapBufferData(std::vector<_Ty> data, GLintptr offset = 0) {
		if (!_isResizable && (data.size() * sizeof(_Ty) + offset) > _bufferSize) {
			return false;
		}

		glNamedBufferSubData(_bufferId, offset, _bufferSize, 0);
		glNamedBufferSubData(_bufferId, offset, data.size() * sizeof(_Ty), data.data());
	}
	
	friend struct VertexArray;
};

struct BufferDescriptor {
	size_t initialValueByteOffset = 0;
	size_t countAttributes;
	BufferFormat formatOfDescrptor;
};