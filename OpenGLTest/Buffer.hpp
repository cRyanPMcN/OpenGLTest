#pragma once
#include "Object.hpp"
#include <GLAD\gl.h>

struct VertexArray;

/// <summary
/// Pass to buffers to specify that size may be changed after creation
/// </summary>
struct MutableBufferT {};


struct __declspec(novtable) Buffer : public Object {
protected:
	GLuint bufferId;
	GLsizeiptr _bufferSize;
	const bool _isNonResizable;

	Buffer(GLsizeiptr bufferSize) : _bufferSize(bufferSize), _isNonResizable(true) {
		glCreateBuffers(1, &bufferId);
		glNamedBufferStorage(bufferId, bufferSize, 0, GL_DYNAMIC_STORAGE_BIT);
	}

	Buffer(GLsizeiptr bufferSize, MutableBufferT) : _bufferSize(bufferSize), _isNonResizable(false) {
		glCreateBuffers(1, &bufferId);
		glNamedBufferData(bufferId, bufferSize, 0, GL_STATIC_DRAW);
	}

	Buffer(GLsizeiptr bufferSize, void* data) : _bufferSize(bufferSize), _isNonResizable(true) {
		glCreateBuffers(1, &bufferId);
		glNamedBufferStorage(bufferId, bufferSize, data, GL_DYNAMIC_STORAGE_BIT);
	}

	Buffer(GLsizeiptr bufferSize, void* data, MutableBufferT) : _bufferSize(bufferSize), _isNonResizable(false) {
		glCreateBuffers(1, &bufferId);
		glNamedBufferData(bufferId, bufferSize, data, GL_STATIC_DRAW);
	}

	virtual ~Buffer() {
		glDeleteBuffers(1, &bufferId);
	}

public:
	/// <summary>
	/// Swap part of buffer for new data
	/// </summary>
	/// <param name="bufferSize"></param>
	/// <param name="data"></param>
	/// <param name="offset"></param>
	/// <returns></returns>
	virtual bool ResetBuffer(GLsizeiptr bufferSize, void* data, GLintptr offset = 0) {
		if (_isNonResizable && bufferSize != _bufferSize) {
			return false;
		}

		// Might be better in some implimentations
		// Tell graphics card driver that the data in the buffer is no longer important
		// This allows to driver to use a more efficient method if it is available
		glNamedBufferSubData(bufferId, offset, _bufferSize, 0);
		// Change data in buffer
		glNamedBufferSubData(bufferId, offset, bufferSize, data);
		return true;
	}
	
	friend struct VertexArray;
};

struct MultiBuffer : public Object {
protected:
	GLuint* bufferIds;
	GLsizei buffersNumber;

	MultiBuffer(GLsizei numberBuffers) : bufferIds(new GLuint[numberBuffers]), buffersNumber(numberBuffers) {
		glCreateBuffers(numberBuffers, bufferIds);
	}

	~MultiBuffer() {
		glDeleteBuffers(buffersNumber, bufferIds);
	}

	friend struct VertexArray;
};