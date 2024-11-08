#pragma once
#include <vector>

enum AttributeType {
	None = 0, Byte, ByteUnsigned, Short, ShortUnsigned, Integer, IntegerUnsigned, Float, FloatFixed, FloatHalf, Double, Packed, PackedUnsigned, PackedFloat
};

/// <summary>
/// Used to calculate the strides and offsets for attributes in a buffer.
/// Bit-Padding is calculated based on alignment of the object in the buffer.
/// </summary>
struct BufferFormat {
	struct Attribute {
		short count;
		AttributeType type;
		bool convertToFloat;
		bool normalize;
		unsigned int offset;
		unsigned int sizeInBytes;

		bool operator==(const Attribute& rhs) const {
			return (count == rhs.count && type == rhs.type && convertToFloat == rhs.convertToFloat && normalize == rhs.normalize && offset == rhs.offset);
		}
	};
protected:
	unsigned int bytesRemaining;
	std::vector<Attribute> attributes;
	unsigned int stride;
	unsigned int alignment;
public:

	/// <param name="align">Alignment of Object that will be stored in the buffer, example: BufferFormat vertFormat(alignof(VertObject));</param>
	BufferFormat(const unsigned int align) : bytesRemaining(align), stride(align), alignment(align) {

	}

	BufferFormat(const BufferFormat& other, const unsigned int align) : bytesRemaining(align), stride(align), alignment(align) {
		for (Attribute const& attribute : other) {
			this->AddAttribute(attribute.count, attribute.type, attribute.convertToFloat, attribute.normalize, attribute.sizeInBytes);
		}
	}

	BufferFormat(const BufferFormat& other) = default;

	BufferFormat(BufferFormat&& other) = default;

	~BufferFormat() = default;

protected:
	/// <summary>
	/// Generic function for adding an attribute
	/// </summary>
	/// <param name="count">Number of attributes</param>
	/// <param name="type">Enum of type</param>
	/// <param name="convertToFloat">Should the value of this attribute be converted to a floating-point</param>
	/// <param name="normalize">Should the value be a normalized floating-point</param>
	/// <param name="attributeStrideInBytes">How many bytes this attribute uses</param>
	/// <returns>Attribute was correct</returns>
	bool AddAttribute(short const count, AttributeType const type, bool const convertToFloat, bool const normalize, unsigned int const attributeStrideInBytes) {
		if (count < 1 || count > 4) {
			return false;
		}
		
		// Check for enough space for a single element of attribute
		// Set the remaining space to 0 to account for padding
		if (bytesRemaining < (attributeStrideInBytes / count)) {
			bytesRemaining = 0;
		}

		// Increase the stride until there is enough space for the attribute
		while (bytesRemaining < attributeStrideInBytes) {
			stride += alignment;
			bytesRemaining += alignment;
		}

		// Create the attribute and add it to the attributes vector
		Attribute attribute;
		attribute.count = count;
		attribute.type = type;
		attribute.convertToFloat = convertToFloat;
		attribute.normalize = normalize;
		// Calculate the offset
		// This sets the position of the attribute in the buffer without relying on the previous positions
		attribute.offset = stride - bytesRemaining;
		attribute.sizeInBytes = attributeStrideInBytes;

		attributes.push_back(attribute);
		bytesRemaining -= attributeStrideInBytes;

		return true;
	}
		
public:
	bool AddByte(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::Byte, convertToFloat, normalize, sizeof(char) * count);
	}

	bool AddShort(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::Short, convertToFloat, normalize, sizeof(short) * count);
	}

	bool AddInteger(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::Integer, convertToFloat, normalize, sizeof(long) * count);
	}

	bool AddUnsignedByte(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::ByteUnsigned, convertToFloat, normalize, sizeof(unsigned char) * count);
	}

	bool AddUnsignedShort(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::ShortUnsigned, convertToFloat, normalize, sizeof(unsigned short) * count);
	}
	bool AddUnsignedInteger(short count, bool convertToFloat = false, bool normalize = false) {
		return AddAttribute(count, AttributeType::IntegerUnsigned, convertToFloat, normalize, sizeof(unsigned int) * count);
	}

	bool AddFixed(short count) {
		return AddAttribute(count, AttributeType::FloatFixed, false, false, sizeof(short) * count);
	}

	bool AddHalfFloat(short count) {
		return AddAttribute(count, AttributeType::FloatHalf, false, false, sizeof(short) * count);
	}

	bool AddFloat(short count) {
		return AddAttribute(count, AttributeType::Float, false, false, sizeof(float) * count);
	}

	bool AddDouble(short count) {
		return AddAttribute(count, AttributeType::Double, false, false, sizeof(double) * count);
	}

	bool AddPackedInteger(bool normalize) {
		return AddAttribute(4, AttributeType::Packed, true, normalize, sizeof(long));
	}

	bool AddPackedUnsigned(bool normalize) {
		return AddAttribute(4, AttributeType::PackedUnsigned, true, normalize, sizeof(unsigned int));
	}

	bool AddPackedFloat() {
		return AddAttribute(3, AttributeType::PackedFloat, false, false, sizeof(unsigned int));
	}

	BufferFormat const& SetAlignment(unsigned int newAlignment) {
		BufferFormat(*this, newAlignment).swap(*this);
		return *this;
	}

	std::vector<Attribute> const& GetAttributes() {
		return attributes;
	}

	/// Number of attributes in the format
	size_t Size() {
		return attributes.size();
	}

	decltype(stride) Stride() {
		return stride;
	}

	decltype(alignment) Alignment() {
		return alignment;
	}
	
	decltype(bytesRemaining) RemainingBytes() {
		return bytesRemaining;
	}

	std::vector<Attribute>::const_iterator begin() const noexcept {
		return attributes.cbegin();
	}

	std::vector<Attribute>::const_iterator end() const noexcept {
		return attributes.cend();
	}

	bool operator==(const BufferFormat& rhs) const {
		bool equivalent = attributes.size() == rhs.attributes.size() && stride == rhs.stride && alignment == rhs.alignment;

		if (equivalent) {
			for (size_t index = 0; index < attributes.size(); ++index) {
				equivalent = attributes[index] == rhs.attributes[index];
				if (!equivalent) {
					break;
				}
			}
		}

		return equivalent;
	}

	bool operator!=(const BufferFormat& rhs) const {
		return !(*this == rhs);
	}

	BufferFormat operator+(const BufferFormat& rhs) const {
		BufferFormat copy(*this);
		copy += rhs;
		return copy;
	}

	BufferFormat& operator+=(const BufferFormat& rhs) {
		attributes.insert(attributes.end(), rhs.attributes.begin(), rhs.attributes.end());
		return *this;
	}

	BufferFormat& swap(BufferFormat& rhs) {
		std::swap(this->bytesRemaining, rhs.bytesRemaining);
		std::swap(this->attributes, rhs.attributes);
		std::swap(this->stride, rhs.stride);
		std::swap(this->alignment, rhs.alignment);
	}
};