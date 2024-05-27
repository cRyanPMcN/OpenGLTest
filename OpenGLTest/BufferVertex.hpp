#pragma once
#include "Object.hpp"
#include "Buffer.hpp"
#include "BufferFormat.hpp"
#include <GLAD\gl.h>
#include <vector>
#include <memory>

class BufferVertex : public Buffer {
	std::shared_ptr<BufferFormat> bufferFormat;
	GLuint _divisor;
public:
	template <class _Ty>
	BufferVertex(std::vector<_Ty> const& vector, std::shared_ptr<BufferFormat> format, unsigned int divisor = 0) : _divisor(divisor), bufferFormat(format), Buffer(sizeof(_Ty) * vector.size(), (void*)vector.data()) {

	}

	template <class _Ty>
	BufferVertex(std::vector<_Ty> const& vector, std::shared_ptr<BufferFormat> format, MutableBufferT, unsigned int divisor = 0) : bufferFormat(format), _divisor(divisor), Buffer(sizeof(_Ty) * vector.size(), (void*)vector.data(), MutableBufferT()) {

	}

	virtual ~BufferVertex() {
	
	}

	void SetDivisor(unsigned int newDivisor) {
		_divisor = newDivisor;
	}

	template <class _Ty>
	bool ResetBuffer(std::vector<_Ty> const& vector) {
		return this->ResetBuffer(sizeof(_Ty) * vector.size(), vector.data());
	}

	bool operator==(const BufferVertex& other) const {
		if (bufferFormat == other.bufferFormat) {
			return true;
		}
		
		return (*bufferFormat == *(other.bufferFormat));
	}

	bool operator!=(const BufferVertex& other) const {
		return !(*this == other);
	}

	friend VertexArray;
};