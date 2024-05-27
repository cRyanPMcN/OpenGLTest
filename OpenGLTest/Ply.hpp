#pragma once
// Created this file to test OpenGL code on a complete model.
// Complete up to index, file is no longer being updated as the code is going to be based on GLTF
#include "BufferFormat.hpp"
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

namespace Ply {
	enum Type {
		Null = 0,
		Char,
		CharUnsigned,
		Short,
		ShortUnsigned,
		Integer,
		IntegerUnsigned,
		Float,
		Double
	};

	static const std::map<std::string, Type> CONVERT_MAP({
		std::pair("char",	Type::Char),
		std::pair("uchar",	Type::CharUnsigned),
		std::pair("short",	Type::Short),
		std::pair("ushort", Type::ShortUnsigned),
		std::pair("int",	Type::Integer),
		std::pair("uint",	Type::IntegerUnsigned),
		std::pair("float",	Type::Float),
		std::pair("double", Type::Double)
		});

	static Type Convert(std::string name) {
		decltype(CONVERT_MAP)::const_iterator it = CONVERT_MAP.find(name);
		if (it == CONVERT_MAP.cend()) {
			return Type::Null;
		}
		return it->second;
	}

	struct PlyFormatAttribute {
		Type type;
		std::string name;
		bool isList;
		Type listType;

		PlyFormatAttribute(std::istream& input) {
			std::string typeText;
			input >> typeText;
			if (typeText == "list") {
				isList = true;
				input >> typeText;
				type = Convert(typeText);
				input >> typeText;
				listType = Convert(typeText);
				input >> name;
			}
			else {
				isList = false;
				listType = Type::Null;
				input >> name;
				type = Convert(typeText);
			}
		}

		bool operator==(const BufferFormat::Attribute& attribute) {
			if (isList == true) {
				return false;
			}
			switch (attribute.type) {
			case AttributeType::Byte:
			case AttributeType::ByteUnsigned:
				return type <= Type::CharUnsigned;
			case AttributeType::Short:
			case AttributeType::ShortUnsigned:
				return type <= Type::ShortUnsigned;
			case AttributeType::Integer:
			case AttributeType::IntegerUnsigned:
				return type <= Type::IntegerUnsigned;
			case AttributeType::Float:
				return type == Type::Float;
			case AttributeType::Double:
				return type == Type::Float || type == Type::Double;
			case AttributeType::FloatFixed:
			case AttributeType::FloatHalf:
			case AttributeType::PackedFloat:
				return type == Type::Float;
			case AttributeType::Packed:
			case AttributeType::PackedUnsigned:
				return type <= Type::IntegerUnsigned;
				//return type == Type::Integer || type == Type::Short || type == Type::Char;
				//return type == Type::IntegerUnsigned || type == Type::ShortUnsigned || type == Type::CharUnsigned;
			default:
				return false;
			}
		}

		bool operator!=(const BufferFormat::Attribute& attribute) {
			return !(*this == attribute);
		}
	};

	struct PlyFormatElement {
		std::string name;
		size_t count;
		std::vector<PlyFormatAttribute> attributes;

		PlyFormatElement() : name(), count(), attributes() {

		}

		PlyFormatElement(std::string n, size_t c) : name(n), count(c) {

		}

		PlyFormatElement(std::istream& input) {
			input >> name >> count;
		}
	};

	struct PlyFormat {
		std::map<std::string, PlyFormatElement> elements;
		size_t lineCount;
		bool isBinary = false;
	};

	bool Parse_Header(std::istream& input, PlyFormat& format) {
		std::string line;
		std::string lastElementFound("");

		while (std::getline(input, line)) {
			++format.lineCount;
			std::istringstream ls(line);
			std::string token;
			ls >> token;
			// Skip comments
			if (token == "comment") {
				continue;
			}

			if (token == "format") {
				std::string s;
				(ls >> s);
				// Program is not going to deal with endian
				if (s == "binary_little_endian") {
					format.isBinary = true;
				}
				else if (s == "binary_big_endian") {
					format.isBinary = true;
				}
			}
			else if (token == "element") {
				PlyFormatElement element(ls);
				lastElementFound = element.name;
				format.elements.emplace(lastElementFound, element);
			}
			else if (token == "property") {
				format.elements[lastElementFound].attributes.emplace_back(ls);
			}
			else if (token == "end_header") {
				break;
			}
			else {
				return false;
			}
		}

		return true;
	}

	template <typename _Ty>
	void Set_Value(void* destination, size_t offsetInBytes, _Ty value) {
		_Ty* dest = reinterpret_cast<_Ty*>(reinterpret_cast<char*>(destination) + offsetInBytes);
		(*dest) = value;
	}

	template <typename _Ty>
	void Parse_Value_Ascii(std::istream& source, void* destination, size_t offsetInBytes) {
		_Ty value;
		source >> value;
		Set_Value<_Ty>(destination, offsetInBytes, value);
	}

	template <typename _Ty>
	void Parse_Value_Binary(std::istream& source, void* destination, size_t offsetInBytes) {
		source.read(((char*)destination + offsetInBytes), sizeof(_Ty));
	}

	void Parse_Binary(std::istream& input, BufferFormat& vertexFormat, void* destination, size_t vertexNumber) {
		unsigned short attributeIndex = 0;
		for (const BufferFormat::Attribute& attribute : vertexFormat.GetAttributes()) {
			for (short idx = 0; idx < attribute.count; ++idx) {
				++attributeIndex;
				switch (attribute.type) {
				case AttributeType::Byte:
					Parse_Value_Binary<char>(input, destination, attribute.offset + (sizeof(char) * idx));
					break;
				case AttributeType::ByteUnsigned:
					Parse_Value_Binary<unsigned char>(input, destination, attribute.offset + (sizeof(unsigned char) * idx));
					break;
				case AttributeType::Short:
					Parse_Value_Binary<short>(input, destination, attribute.offset + (sizeof(short) * idx));
					break;
				case AttributeType::ShortUnsigned:
					Parse_Value_Binary<unsigned short>(input, destination, attribute.offset + (sizeof(unsigned short) * idx));
					break;
				case AttributeType::Integer:
					Parse_Value_Binary<int>(input, destination, attribute.offset + (sizeof(int) * idx));
					break;
				case AttributeType::IntegerUnsigned:
					Parse_Value_Binary<unsigned int>(input, destination, attribute.offset + (sizeof(unsigned int) * idx));
					break;
				case AttributeType::Float:
					Parse_Value_Binary<float>(input, destination, attribute.offset + (sizeof(float) * idx));
					break;
				case AttributeType::Double:
					Parse_Value_Binary<double>(input, destination, attribute.offset + (sizeof(double) * idx));
					break;
				}

				if (input.fail()) {
					throw std::runtime_error("Parse_Ply_File::Parse_Value_Binary: Ply vertex number: " + std::to_string(vertexNumber) + ", attribute " + std::to_string(attributeIndex + 1) + " failed to read.");
				}
			}
		}
	}

	template <class _idxTy>
	_idxTy Parse_Index_Binary(std::istream& input, Type& plyType) {
		_idxTy value = 0;
		switch (plyType) {
		case Type::Char:
			char c;
			input.read((char*)&c, sizeof(char));
			value = c;
			break;
		case Type::CharUnsigned:
			unsigned char uc;
			input.read((char*)&uc, sizeof(unsigned char));
			value = uc;
			break;
		case Type::Short:
			short s;
			input.read((char*)&s, sizeof(short));
			value = s;
			break;
		case Type::ShortUnsigned:
			unsigned short us;
			input.read((char*)&us, sizeof(unsigned short));
			value = us;
			break;
		case Type::Integer:
			int i;
			input.read((char*)&i, sizeof(int));
			value = i;
			break;
		case Type::IntegerUnsigned:
			unsigned int ui;
			input.read((char*)&ui, sizeof(unsigned int));
			value = ui;
			break;
		case Type::Float:
			float f;
			input.read((char*)&f, sizeof(float));
			value = f;
			break;
		case Type::Double:
			double d;
			input.read((char*)&d, sizeof(double));
			value = d;
			break;
		}

		return value;
	}

	template <class _Ty, class _idxType>
	void Parse_Ply_File(std::istream& plySource, BufferFormat& vertexFormat, std::vector<_Ty>& vertexBuffer, std::map<_idxType, std::vector<_idxType>>& indexBuffer) {
		if (!vertexBuffer.empty() || !indexBuffer.empty()) {
			throw std::invalid_argument("Parse_Ply_File: vertexBuffer and indexBuffer must be empty.");
		}
		if (vertexFormat.stride != sizeof(_Ty)) {
			throw std::invalid_argument("Parse_Ply_File: vertexFormat stride is not equal to the size of the Vertex object.");
		}

		for (const BufferFormat::Attribute& attribute : vertexFormat.GetAttributes()) {
			switch (attribute.type) {
			case AttributeType::FloatFixed:
			case AttributeType::FloatHalf:
			case AttributeType::PackedFloat:
			case AttributeType::Packed:
			case AttributeType::PackedUnsigned:
				throw std::invalid_argument("Parse_Ply_File: Ply Parser does not support half-float, fixed-float, or packed formats directly, it is recommended that you import to a intermediary vertex and convert to the correct type.");
			}
		}

		PlyFormat format;
		format.lineCount = 0;
		std::string line;

		if (!std::getline(plySource, line)) {
			throw std::invalid_argument("Parse_Ply_File: plySource is empty.");
		}
		else {
			++format.lineCount;
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			if (line != "ply") {
				throw std::invalid_argument("Parse_Ply_File: plySource is not a Ply file.");
			}
		}

		if (!Parse_Header(plySource, format)) {
			throw std::invalid_argument("Parse_Ply_File::Parse_Header: Unexpected token found at line " + std::to_string(format.lineCount) + ".");
		}

		if (format.elements.find("vertex") == format.elements.end()) {
			throw std::runtime_error("Parse_Ply_File: Ply file is missing a 'vertex' element.");
		}

		if (format.elements.find("face") == format.elements.end()) {
			throw std::runtime_error("Parse_Ply_File: Ply file is missing a 'face' element.");
		}

		// Validate Ply Format is convertible to the BufferFormat
		PlyFormatElement& vertexElement = format.elements["vertex"];
		{
			size_t attributeIdx = 0;
			for (const BufferFormat::Attribute& attribute : vertexFormat.GetAttributes()) {
				for (short i = 0; i < attribute.count; ++i) {
					if (vertexElement.attributes[attributeIdx] != attribute) {
						throw std::runtime_error("Parse_Ply_File: Ply file attributes are not convertible to vertexFormat attributes");
					}
					++attributeIdx;
				}
			}
		}

		PlyFormatElement& indexElement = format.elements["face"];
		vertexBuffer.resize(vertexElement.count);
		if (format.isBinary) {
			// Parse Vertexes into the vertexBuffer
			for (size_t i = 0; i < vertexElement.count; ++i) {
				++format.lineCount;
				void* data = &(vertexBuffer[i]);
				Parse_Binary(plySource, vertexFormat, data, i);
			}

			if (indexElement.attributes.size() != 1 && !indexElement.attributes[0].isList) {
				throw std::runtime_error("Parse_Ply_File: Ply face element must have one attribute and that attribute must be a list.");
			}

			for (size_t ei = 0; ei < indexElement.count; ++ei) {
				_idxType listCount = Parse_Index_Binary<_idxType>(plySource, indexElement.attributes[0].type);
				if (plySource.fail()) {
					throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + ", index element " + std::to_string(indexElement.count) + " failed to parse listCount.");
				}

				for (_idxType idx = 0; idx < listCount; ++idx) {
					_idxType value = Parse_Index_Binary<_idxType>(plySource, indexElement.attributes[0].listType);
					if (plySource.fail()) {
						throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + ", index element " + std::to_string(indexElement.count) + " index " + std::to_string(idx + 1) + " failed to parse.");
					}
					indexBuffer[listCount].push_back(value);
				}
			}
		}
		else {
			if (indexElement.attributes.size() != 1 && !indexElement.attributes[0].isList) {
				throw std::runtime_error("Parse_Ply_File: Ply face element must have one attribute and that attribute must be a list.");
			}

			// Parse Vertexes into the vertexBuffer
			for (size_t i = 0; i < vertexElement.count; ++i) {
				if (!std::getline(plySource, line)) {
					throw std::runtime_error("Parse_Ply_File: Unexpected end of file at " + std::to_string(format.lineCount) + " expected a vertex element.");
				}

				++format.lineCount;
				void* data = &(vertexBuffer[i]);
				std::istringstream str(line);
				unsigned short attributeIndex = 0;
				for (const BufferFormat::Attribute& attribute : vertexFormat.GetAttributes()) {
					for (short idx = 0; idx < attribute.count; ++idx) {
						++attributeIndex;
						if (str.eof()) {
							throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + " has " + std::to_string(attributeIndex) + " attributes, expected " + std::to_string(attribute.count) + " attributes.");
						}

						switch (attribute.type) {
						case AttributeType::Byte:
							short b;
							str >> b;
							if (str.fail()) {
								throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + " attribute " + std::to_string(attributeIndex) + " failed to parse.");
							}
							Set_Value(data, attribute.offset + (sizeof(char) * idx), (unsigned char)b);
							break;
						case AttributeType::ByteUnsigned:
							unsigned short ub;
							str >> ub;
							if (str.fail()) {
								throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + " attribute " + std::to_string(attributeIndex) + " failed to parse.");
							}
							Set_Value(data, attribute.offset + (sizeof(unsigned char) * idx), (unsigned char)ub);
							break;
						case AttributeType::Short:
							Parse_Value_Ascii<short>(str, data, attribute.offset + (sizeof(short) * idx));
							break;
						case AttributeType::ShortUnsigned:
							Parse_Value_Ascii<unsigned short>(str, data, attribute.offset + (sizeof(unsigned short) * idx));
							break;
						case AttributeType::Integer:
							Parse_Value_Ascii<int>(str, data, attribute.offset + (sizeof(int) * idx));
							break;
						case AttributeType::IntegerUnsigned:
							Parse_Value_Ascii<unsigned int>(str, data, attribute.offset + (sizeof(unsigned int) * idx));
							break;
						case AttributeType::Float:
							Parse_Value_Ascii<float>(str, data, attribute.offset + (sizeof(float) * idx));
							break;
						case AttributeType::Double:
							Parse_Value_Ascii<double>(str, data, attribute.offset + (sizeof(double) * idx));
							break;
						}

						if (str.fail()) {
							throw std::runtime_error("Parse_Ply_File::Parse_Value_Ascii Ply vertex at line " + std::to_string(format.lineCount) + ", attribute " + std::to_string(attributeIndex) + " failed to parse.");
						}
					}
				}

				if (!str.eof()) {
					throw std::runtime_error("Parse_Ply_File: Ply vertex at line " + std::to_string(format.lineCount) + " missing expected end of line.");
				}
			}

			for (size_t ei = 0; ei < indexElement.count; ++ei) {
				++format.lineCount;

				if (!std::getline(plySource, line)) {
					throw std::runtime_error("Parse_Ply_File: Unexpected end of file at " + std::to_string(format.lineCount) + " expected a face element.");
				}
				++format.lineCount;
				_idxType attributeCount;
				std::istringstream str(line);

				str >> attributeCount;
				if (str.fail()) {
					throw std::runtime_error("Parse_Ply_File: Ply index at line " + std::to_string(format.lineCount) + " failed to parse Face list size.");
				}

				for (_idxType idx = 0; idx < attributeCount; ++idx) {
					if (str.eof()) {
						throw std::runtime_error("Parse_Ply_File: Ply index at line " + std::to_string(format.lineCount) + ", index " + std::to_string(idx + 1) + " unexpected end of line.");
					}
					unsigned int value;
					str >> value;
					if (str.fail()) {
						throw std::runtime_error("Parse_Ply_File: Ply index at line " + std::to_string(format.lineCount) + ", index element " + std::to_string(indexElement.count) + " index " + std::to_string(idx + 1) + " failed to parse.");
					}
					indexBuffer[attributeCount].push_back(value);
				}
			}
		}

	}
}