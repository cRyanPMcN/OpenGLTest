#pragma once
#include "Object.hpp"
#include "BufferVertex.hpp"
#include "BufferIndex.hpp"
#include <GLAD\gl.h>

struct VertexArray : public Object {
public:
	struct Binding {
		VertexArray& vertexArray;
		GLuint idBinding;
		std::vector<GLuint> idAttributes;
		std::shared_ptr<BufferVertex> buffer;
		std::shared_ptr<BufferFormat> formatBuffer;

		Binding(VertexArray& bindingSource, std::shared_ptr<BufferFormat> bufferFormat) : vertexArray(bindingSource), idBinding(0), buffer(nullptr), formatBuffer(bufferFormat) {
			GLuint attributeId = 0;
			glVertexArrayVertexBuffer(vertexArray.idVertexArray, idBinding, 0, 0, formatBuffer->stride);
			// Bind attribute to buffer binding
			for (BufferFormat::Attribute const& attribute : *formatBuffer) {
				glVertexArrayAttribBinding(vertexArray.idVertexArray, attributeId, idBinding);
				GLenum type = ConvertType(attribute.type);

				switch (attribute.type) {
				case AttributeType::FloatFixed:
				case AttributeType::FloatHalf:
				case AttributeType::Float:
				case AttributeType::Packed:
				case AttributeType::PackedUnsigned:
				case AttributeType::PackedFloat:
					glVertexArrayAttribFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.normalize, attribute.offset);
					break;
				case AttributeType::Byte:
				case AttributeType::ByteUnsigned:
				case AttributeType::Short:
				case AttributeType::ShortUnsigned:
				case AttributeType::Integer:
				case AttributeType::IntegerUnsigned:
					if (attribute.convertToFloat) {
						glVertexArrayAttribFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.normalize, attribute.offset);
					}
					else {
						glVertexArrayAttribIFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.offset);
					}
					break;
				case AttributeType::Double:
					glVertexArrayAttribLFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.offset);
					break;
				}
				idAttributes.push_back(attributeId);
				++attributeId;
			}
		}

		Binding(VertexArray& bindingSource, GLuint bindingId, GLuint& attributeId, std::shared_ptr<BufferFormat> bufferFormat) : vertexArray(bindingSource), idBinding(bindingId), buffer(nullptr), formatBuffer(bufferFormat) {
			glVertexArrayVertexBuffer(vertexArray.idVertexArray, idBinding, 0, 0, formatBuffer->stride);
			// Bind attribute to buffer binding
			for (BufferFormat::Attribute const& attribute : *formatBuffer) {
				glVertexArrayAttribBinding(vertexArray.idVertexArray, attributeId, idBinding);
				GLenum type = ConvertType(attribute.type);

				switch (attribute.type) {
				case AttributeType::FloatFixed:
				case AttributeType::FloatHalf:
				case AttributeType::Float:
				case AttributeType::Packed:
				case AttributeType::PackedUnsigned:
				case AttributeType::PackedFloat:
					glVertexArrayAttribFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.normalize, attribute.offset);
					break;
				case AttributeType::Byte:
				case AttributeType::ByteUnsigned:
				case AttributeType::Short:
				case AttributeType::ShortUnsigned:
				case AttributeType::Integer:
				case AttributeType::IntegerUnsigned:
					if (attribute.convertToFloat) {
						glVertexArrayAttribFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.normalize, attribute.offset);
					}
					else {
						glVertexArrayAttribIFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.offset);
					}
					break;
				case AttributeType::Double:
					glVertexArrayAttribLFormat(vertexArray.idVertexArray, attributeId, attribute.count, type, attribute.offset);
					break;
				}
				idAttributes.push_back(attributeId);
				++attributeId;
			}
		}

		Binding(const Binding&) = default;

		Binding(Binding&&) = default;

		bool SetBuffer(const std::shared_ptr<BufferVertex>& sourceBuffer) {
			// Check that user is not trying to set the same buffer twice
			if (buffer == sourceBuffer) {
				return true;
			}

			// Disable attributes if the buffer is nullptr
			if (sourceBuffer == nullptr) {
				for (GLuint attribute : idAttributes) {
					glDisableVertexArrayAttrib(vertexArray.idVertexArray, attribute);
				}
			}
			else {
				// Verify that the format of the sourceBuffer is equivalent to the buffer that is being replaced
				if (formatBuffer != sourceBuffer->bufferFormat && (*(sourceBuffer->bufferFormat) != *(formatBuffer))) {
					return false;
				}

				glVertexArrayVertexBuffer(vertexArray.idVertexArray, idBinding, sourceBuffer->bufferId, 0, formatBuffer->stride);
				glVertexArrayBindingDivisor(vertexArray.idVertexArray, idBinding, sourceBuffer->_divisor);
				if (buffer == nullptr) {
					for (GLuint attribute : idAttributes) {
						glEnableVertexArrayAttrib(vertexArray.idVertexArray, attribute);
					}
				}
			}

			buffer = sourceBuffer;
			return true;
		}

		bool operator==(const Binding& rhs) const {
			return idBinding == rhs.idBinding && formatBuffer == rhs.formatBuffer && std::equal(idAttributes.cbegin(), idAttributes.cend(), rhs.idAttributes.cbegin());
		}

		bool operator==(const std::shared_ptr<BufferFormat>& rhs) const {
			return ((formatBuffer == rhs) || (*formatBuffer == *rhs));
		}

		bool operator!=(const std::shared_ptr<BufferFormat>& rhs) const {
			return !(*this == rhs);
		}

		Binding& operator=(const Binding&) = default;

		Binding& operator=(Binding&&) = default;
	};

	using container_bindings = std::vector<Binding>;
protected:
	GLuint idVertexArray;
	std::vector<Binding> bindings;
	std::shared_ptr<BufferIndex> bufferIndex;
public:
	VertexArray(std::shared_ptr<BufferFormat>& vertexFormat) {
		glCreateVertexArrays(1, &idVertexArray);
		bindings.emplace_back(*this, vertexFormat);
	}

	VertexArray(std::vector<std::shared_ptr<BufferFormat>>& vertexFormats) {
		glCreateVertexArrays(1, &idVertexArray);
		bindings.reserve(vertexFormats.size());

		GLuint attributeId = 0;

		for (size_t idx = 0; idx < vertexFormats.size(); ++idx) {
			bindings.emplace_back(*this, idx, attributeId, vertexFormats[idx]);
		}
	}

	~VertexArray() {
		glDeleteVertexArrays(1, &idVertexArray);
	}

	inline static GLenum ConvertType(AttributeType type) {
		switch (type) {
		case AttributeType::Byte:
			return GL_BYTE;
		case AttributeType::ByteUnsigned:
			return GL_UNSIGNED_BYTE;
		case AttributeType::Short:
			return GL_SHORT;
		case AttributeType::ShortUnsigned:
			return GL_UNSIGNED_SHORT;
		case AttributeType::Integer:
			return GL_INT;
		case AttributeType::IntegerUnsigned:
			return GL_UNSIGNED_INT;
		case AttributeType::FloatFixed:
			return GL_FIXED;
		case AttributeType::FloatHalf:
			return GL_HALF_FLOAT;
		case AttributeType::Float:
			return GL_FLOAT;
		case AttributeType::Double:
			return GL_DOUBLE;
		case AttributeType::Packed:
			return GL_INT_2_10_10_10_REV;
		case AttributeType::PackedUnsigned:
			return GL_UNSIGNED_INT_2_10_10_10_REV;
		case AttributeType::PackedFloat:
			return GL_UNSIGNED_INT_10F_11F_11F_REV;
		default:
			// WHAT DID YOU DOOOOOOOOOO
			// WHY DID YOU DO THIS
			// YOU FOOL
			throw std::exception("VertexArray::ConvertType, you know what you did.");
		};
	}

	std::vector<Binding> const& GetBindings() {
		return bindings;
	}

	bool SetBuffer(std::shared_ptr<BufferVertex> sourceBuffer, Binding& binding) {
		return binding.SetBuffer(sourceBuffer);
	}

	bool SetBuffer(std::shared_ptr<BufferVertex> sourceBuffer, const Binding& binding) {
		std::vector<Binding>::iterator bind = std::find(bindings.begin(), bindings.end(), binding);
		if (bind == bindings.end()) {
			return false;
		}

		return SetBuffer(sourceBuffer, const_cast<Binding&>(binding));
	}

	bool SetBuffer(std::shared_ptr<BufferVertex> sourceBuffer, GLuint bindingIndex) {
		if (bindingIndex > bindings.size()) {
			return false;
		}

		return SetBuffer(sourceBuffer, bindings[bindingIndex]);
	}

	bool SetIndexBuffer(std::shared_ptr<BufferIndex> buffer) {
		if (buffer == nullptr) {
			return false;
		}

		if (bufferIndex != buffer) {
			glVertexArrayElementBuffer(idVertexArray, buffer->bufferId);
			bufferIndex = buffer;
		}

		return true;
	}	

	void Bind() const {
		glBindVertexArray(idVertexArray);
	}

	size_t Size() const noexcept {
		return bindings.size();
	}

	friend int main(void);
};