#pragma once
#include "Buffer.hpp"
#include <GLAD\gl.h>
#include <vector>

struct IndexBufferSub {
	GLsizeiptr subBufferSize;
	GLintptr bufferOffset;

	IndexBufferSub(std::vector<unsigned int> const& indexes, GLintptr offset) : subBufferSize(sizeof(unsigned int) * indexes.size()), bufferOffset(offset) {

	}
};

class BufferIndex : public Buffer {
	GLsizeiptr CalculateBufferSize(std::vector<std::vector<unsigned int>> const& indexes) {
		subBuffers.reserve(indexes.size());
		GLsizeiptr bufferSize = 0;
		for (std::remove_reference_t<decltype(indexes)>::const_reference index : indexes) {
			subBuffers.emplace_back(index, bufferSize);
			bufferSize += sizeof(std::remove_reference_t<decltype(index)>::value_type) * index.size();
		}
		return bufferSize;
	}
public:
	std::vector<IndexBufferSub> subBuffers;
	BufferIndex(std::vector<unsigned int> const& indexes) : subBuffers(), Buffer(sizeof(std::remove_reference_t<decltype(indexes)>::value_type) * indexes.size(), (void*)indexes.data()) {
		subBuffers.emplace_back(indexes, 0);
	}

	BufferIndex(std::vector<unsigned int> const& indexes, MutableBufferT makeMutable) : subBuffers(), Buffer(sizeof(std::remove_reference_t<decltype(indexes)>::value_type)* indexes.size(), (void*)indexes.data(), makeMutable) {
		subBuffers.emplace_back(indexes, 0);
	}

	BufferIndex(std::vector<std::vector<unsigned int>> const& indexes) : subBuffers(), Buffer(CalculateBufferSize(indexes)) {
		for (size_t idx = 0; idx < subBuffers.size(); ++idx) {
			glNamedBufferSubData(bufferId, subBuffers[idx].bufferOffset, subBuffers[idx].subBufferSize, indexes[idx].data());
		}
	}

	BufferIndex(std::vector<std::vector<unsigned int>> const& indexes, MutableBufferT makeMutable) : subBuffers(), Buffer(CalculateBufferSize(indexes), makeMutable) {
		for (size_t idx = 0; idx < subBuffers.size(); ++idx) {
			glNamedBufferSubData(bufferId, subBuffers[idx].bufferOffset, subBuffers[idx].subBufferSize, indexes[idx].data());
		}
	}

	~BufferIndex() {

	}

	friend VertexArray;
};