#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <charconv>
#include <iomanip>
#include <map>
#include <memory>

namespace JsonParse {
		enum class Type : char {
		None,
		Object,
		Array,
		String,
		Boolean,
		Integer,
		Number,
		Null
	};

	struct __declspec(novtable) JsonElement {
		Type type;

		JsonElement() : type(Type::None) {

		}

		JsonElement(Type cType) : type(cType) {

		}

		JsonElement(JsonElement const&) = default;
		JsonElement(JsonElement&&) = default;

		// This line causes a memory overhead increase
		// However removing the virtual leads to "UNDEFINED BEHAVIOUR"
		virtual ~JsonElement() = default;
	};


	struct JsonString : JsonElement {
		using value_type = std::string;
		static const Type Class_Type() {
			return Type::String;
		}
		std::string value;

		JsonString(std::string const& val) : value(val), JsonElement(Type::String) {

		}

		JsonString(JsonString const&) = default;
		JsonString(JsonString&&) = default;

		~JsonString() = default;
	};

	struct JsonBoolean : JsonElement {
		using value_type = bool;
		static const Type Class_Type() {
			return Type::Boolean;
		}
		bool value;

		JsonBoolean(bool val) : value(val), JsonElement(Type::Boolean) {

		}

		JsonBoolean(JsonBoolean const&) = default;
		JsonBoolean(JsonBoolean&&) = default;

		~JsonBoolean() = default;
	};

	struct JsonInteger : JsonElement {
		using value_type = intmax_t;
		static const Type Class_Type() {
			return Type::Integer;
		}
		intmax_t value;

		JsonInteger(intmax_t val) : value(val), JsonElement(Type::Integer) {

		}

		JsonInteger(JsonInteger const&) = default;
		JsonInteger(JsonInteger&&) = default;

		~JsonInteger() = default;
	};

	struct JsonNumber : JsonElement {
		using value_type = double;
		static const Type Class_Type() {
			return Type::Number;
		}
		double value;

		JsonNumber(double val) : value(val), JsonElement(Type::Number) {

		}

		JsonNumber(JsonNumber const&) = default;
		JsonNumber(JsonNumber&&) = default;

		~JsonNumber() = default;
	};

	struct JsonNull : JsonElement {
		static Type Class_Type() {
			return Type::Null;
		}

		JsonNull() : JsonElement(Type::Null) {

		}

		JsonNull(JsonNull const&) = default;
		JsonNull(JsonNull&&) = default;

		~JsonNull() = default;
	};

	struct JsonObject : JsonElement {
		static const Type Class_Type() {
			return Type::Object;
		}
		using key_type = std::string;
		using value_type = std::shared_ptr<JsonElement>;
		using container_type = std::map<key_type, value_type>;
		using pair_type = container_type::value_type;
		container_type attributes;

		JsonObject() : JsonElement(Type::Object) {

		}

		JsonObject(JsonObject const&) = default;
		JsonObject(JsonObject&&) = default;

		~JsonObject() = default;

		std::shared_ptr<JsonElement> Find(std::string const& key) {
			container_type::const_iterator findResult = attributes.find(key);
			if (findResult != attributes.cend()) {
				return findResult->second;
			}

			return std::shared_ptr<JsonElement>(nullptr);
		}

		JsonObject& operator=(JsonObject const&) = default;
		JsonObject& operator=(JsonObject&&) = default;

		size_t Size() const noexcept {
			return attributes.size();
		}

		std::shared_ptr<JsonElement>& operator[](std::string const& str) noexcept {
			return attributes[str];
		}
	};

	struct JsonArray : JsonElement {
		static const Type Class_Type() {
			return Type::Array;
		}
		using value_type = std::shared_ptr<JsonElement>;
		using container_type = std::vector<std::shared_ptr<JsonElement>>;
		container_type values;

		JsonArray() : JsonElement(Type::Array) {

		}

		JsonArray(JsonArray const&) = default;
		JsonArray(JsonArray&&) = default;

		~JsonArray() = default;

		size_t Size() const noexcept {
			return values.size();
		}

		std::shared_ptr<JsonElement>& operator[](size_t idx) noexcept {
			return values[idx];
		}
	};

#define FILE_FUNCTION_LINE std::string(__FILE__) + ':' + std::string(__FUNCTION__) + '@' + std::to_string(__LINE__)

	struct JsonReader {
		struct Statistics {
			size_t lines;
			size_t columns;
			size_t objects;
			size_t arrays;
			size_t strings;
			size_t booleans;
			size_t integers;
			size_t doubles;
			size_t nulls;

			Statistics() : lines(1), columns(1), objects(), arrays(), strings(), booleans(), integers(), doubles(), nulls() {

			}

			void Increment(char c = 0) {
				if (c == '\n') {
					columns = 0;
					++lines;
				}
				else {
					++columns;
				}
			}
		} fileStats;

		std::string jsonSource;
		std::shared_ptr<JsonElement> rootNode;

		JsonReader(std::ifstream& file) : rootNode(nullptr), jsonSource(""), fileStats() {
			if (file.is_open()) {
				std::stringstream stringSource;
				stringSource << file.rdbuf();
				jsonSource = stringSource.str();
				file.close();
				ParseJson();
			}
		}

		JsonReader(std::ifstream&& file) : rootNode(nullptr), jsonSource(""), fileStats() {
			if (file.is_open()) {
				std::stringstream stringSource;
				stringSource << file.rdbuf();
				jsonSource = stringSource.str();
				file.close();
				
				ParseJson();
			}
		}

		JsonReader(std::filesystem::path const& jsonPath) : JsonReader(std::ifstream(jsonPath, std::ios::binary)) {

		}

		JsonReader(std::filesystem::path&& jsonPath) : JsonReader(std::ifstream(jsonPath, std::ios::binary)) {

		}

		JsonReader(std::string const& source) : rootNode(nullptr), jsonSource(source.cbegin(), source.cend()), fileStats() {
			ParseJson();
		}

		~JsonReader() {

		}

		static std::pair<std::shared_ptr<JsonElement>, JsonReader::Statistics> Parse_Json(std::filesystem::path const& filePath) {
			JsonReader file(filePath);
			return std::pair<std::shared_ptr<JsonElement>, JsonReader::Statistics>(file.rootNode, file.fileStats);
		}

	protected:
		bool IsJsonSpace(char const c) {
			return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
		}

		const std::string GetStandardErrorMessage(std::string::const_iterator& sourceIter) {
			std::string::const_iterator iterCopy = sourceIter;
			sourceIter = std::find(sourceIter, jsonSource.cend(), '\n');
			std::string line(iterCopy, sourceIter);
			return "[Line:" + std::to_string(fileStats.lines) + ", Column:" + std::to_string(fileStats.columns) + "]\nRemaining Line:" + line;
		}

		void SkipWhiteSpace(std::string::const_iterator& sourceIter) {
			while ((sourceIter != jsonSource.cend()) && (*sourceIter == ' ' || *sourceIter == '\n' || *sourceIter == '\t' || *sourceIter == '\r')/*isspace(c)*/) {
				fileStats.Increment(*sourceIter);
				++sourceIter;
			}

			if (sourceIter == jsonSource.cend()) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected End-Of-File encountered at " + GetStandardErrorMessage(sourceIter));
			}

			// Check for comments
			if (*sourceIter == '/') {
				switch (*(sourceIter + 1)) {
				case '/':
				{
					std::string::const_iterator iterCopy = sourceIter;
					sourceIter = std::find(sourceIter, jsonSource.cend(), '\n');
					if (sourceIter == jsonSource.cend()) {
						throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected End-Of-File encountered while skipping comment at " + GetStandardErrorMessage(iterCopy));
					}
					++fileStats.lines;
					fileStats.columns = 0;
					SkipWhiteSpace(sourceIter);
					break;
				}
				case '*':
				{
					while (*sourceIter != '/') {
						std::string::const_iterator iterCopy = sourceIter;
						sourceIter = std::find(sourceIter, jsonSource.cend(), '*');

						if (sourceIter == jsonSource.cend()) {
							throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected End-Of-File encountered while skipping comment at " + GetStandardErrorMessage(iterCopy));
						}

						std::string::const_reverse_iterator::difference_type numberLines = std::count(iterCopy, sourceIter, '\n');
						fileStats.lines += numberLines;
						// + 1 is to add back in the '*' which is removed by getline
						if (numberLines != 0) {
							iterCopy = sourceIter;
							while (*iterCopy != '\n') {
								--iterCopy;
							}
							fileStats.columns = std::distance(iterCopy, sourceIter) + 1;
						}
						else {
							fileStats.columns += std::distance(iterCopy, sourceIter);
						}
					}

					// Increment columns to acount for the ending forward slash at the end of the comment block
					++fileStats.columns;
					SkipWhiteSpace(sourceIter);
					break;
				}
				}
			}
		}

		std::string GetString(std::string::const_iterator& sourceIter) {
			std::string value;
			while (++sourceIter != jsonSource.cend()) {
				fileStats.Increment(*sourceIter);
				if (*sourceIter == '\\') {
					if ((sourceIter + 1) == jsonSource.cend()) {
						// Exception, invalid escape sequence
					}
					switch (*(sourceIter + 1)) {
					case '"':
					case '\\':
					case '/':
					case 'b':
					case 'f':
					case 'n':
					case 'r':
					case 't':
						fileStats.columns += 2;
						value += '\\' + *(++sourceIter);
						break;
					case 'u':
					{
						++sourceIter;
						int character = 0;
						fileStats.columns += 2;
						for (int i = 0; i < 4; ++i) {
							if ((++sourceIter != jsonSource.cend()) && isxdigit(*sourceIter)) {
								character *= 16;
								if (*sourceIter <= '9') {
									character += *sourceIter - '0';
								}
								else {
									character += toupper((unsigned char)*sourceIter) - 'A' + 10;
								}
							}
							else {
								throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to parse unicode literal at " + GetStandardErrorMessage(sourceIter));
							}
						}
						if (character < 128) {
							value += (char)character;
						}
						else if (character < 2048) {
							value += (char)(192 | (character >> 6));
							value += (char)(128 | (character & 63));
						}
						else {
							value += (char)(224 | (character >> 12));
							value += (char)(128 | ((character >> 6) & 63));
							value += (char)(128 | (character & 63));
						}
					}
					break;
					default:
						throw std::runtime_error(FILE_FUNCTION_LINE + ": Invalid escape character '" + *sourceIter + "' at " + GetStandardErrorMessage(sourceIter));
					}
				}
				else {
					if (*sourceIter == '"') {
						break;
					}
					else {
						value += *sourceIter;
					}
				}
			}

			if (sourceIter == jsonSource.cend()) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected End-Of-File encountered at " + GetStandardErrorMessage(sourceIter));
			}

			// Count the end quote
			++fileStats.columns;
			++sourceIter;
			return value;
		}

		std::shared_ptr<JsonString> ParseString(std::string::const_iterator& sourceIter) {
			++fileStats.strings;
			return std::make_shared<JsonString>(GetString(sourceIter));
		}

		std::shared_ptr<JsonBoolean> ParseTrue(std::string::const_iterator& sourceIter) {
			char check[] = "true";
			for (int i = 0; i < strlen(check); ++i) {
				if ((sourceIter + i) == jsonSource.cend() || tolower((unsigned char)*(sourceIter + i)) != check[i]) {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Did not find \"true\" at " + GetStandardErrorMessage(sourceIter));
				}
				++fileStats.columns;
			}

			sourceIter += strlen(check);
			++fileStats.booleans;
			return std::make_shared<JsonBoolean>(true);
		}

		std::shared_ptr<JsonBoolean> ParseFalse(std::string::const_iterator& sourceIter) {
			char check[] = "false";
			for (int i = 0; i < strlen(check); ++i) {
				if ((sourceIter + i) == jsonSource.cend() || tolower((unsigned char)*(sourceIter + i)) != check[i]) {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Did not find \"false\" at " + GetStandardErrorMessage(sourceIter));
				}
				++fileStats.columns;
			}

			sourceIter += strlen(check);
			++fileStats.booleans;
			return std::make_shared<JsonBoolean>(false);
		}

		std::shared_ptr<JsonNull> ParseNull(std::string::const_iterator& sourceIter) {
			char check[] = "null";
			for (int i = 0; i < strlen(check); ++i) {
				if ((sourceIter + i) == jsonSource.cend() || tolower((unsigned char)*(sourceIter + i)) != check[i]) {
					throw std::runtime_error("JsonFile::ParseNull: Did not find \"null\" at " + GetStandardErrorMessage(sourceIter));
				}
				++fileStats.columns;
			}

			sourceIter += strlen(check);
			++fileStats.nulls;
			return std::make_shared<JsonNull>();
		}

		std::shared_ptr<JsonElement> ParseNumber(std::string::const_iterator& sourceIter) {
			bool isDouble = false;
			std::string::const_iterator iterCopy = sourceIter;
			while (++sourceIter != jsonSource.cend() && ((*sourceIter >= '0' && *sourceIter <= '9') || *sourceIter == 'E' || *sourceIter == 'e' || *sourceIter == '+' || *sourceIter == '-' || *sourceIter == '.')) {
				if (*sourceIter == '.') {
					isDouble = true;
				}
				++fileStats.columns;
			}

			if (sourceIter == jsonSource.cend()) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected End-Of-File encountered at " + GetStandardErrorMessage(sourceIter));
			}

			std::string value(iterCopy, sourceIter);

			// char_conv does not accept strings with + at the front
			// So we strip it as it does not change the value of the number
			if (value[0] == '+') {
				value.erase(0, 1);
			}

			if (!isDouble) {
				intmax_t intVal;
				const std::from_chars_result res = std::from_chars(value.c_str(), value.c_str() + value.size(), intVal);
				if (res.ec == std::errc::invalid_argument) {
					fileStats.columns -= value.size();
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to parse \"" + value + "\" as integer at " + GetStandardErrorMessage(sourceIter));
				}
				++fileStats.integers;
				return std::make_shared<JsonInteger>(intVal);
			}
			else {
				double flVal;
				const std::from_chars_result res = std::from_chars(value.c_str(), value.c_str() + value.size(), flVal);
				if (res.ec == std::errc::invalid_argument) {
					fileStats.columns -= value.size();
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to parse \"" + value + "\" as double at " + GetStandardErrorMessage(sourceIter));
				}
				++fileStats.doubles;
				return std::make_shared<JsonNumber>(flVal);
			}
		}

		std::shared_ptr<JsonObject> ParseObject(std::string::const_iterator& sourceIter) {
			std::shared_ptr<JsonObject> retVal = std::make_shared<JsonObject>();
			++sourceIter;
			SkipWhiteSpace(sourceIter);

			if (*sourceIter == '}') {
				// Empty Object
				++fileStats.objects;
				++fileStats.columns;
				++sourceIter;
				return retVal;
			}

			do {
				SkipWhiteSpace(sourceIter);

				if (*sourceIter != '"') {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to find double quote for attribute name at " + GetStandardErrorMessage(sourceIter));
				}

				std::string attributeName = GetString(sourceIter);

				if (retVal->attributes.find(attributeName) != retVal->attributes.cend()) {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Found repeat object attribute name at " + GetStandardErrorMessage(sourceIter));
				}

				SkipWhiteSpace(sourceIter);

				if (*sourceIter != ':') {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to get name-value seperator ':' at " + GetStandardErrorMessage(sourceIter));
				}

				++sourceIter;
				++fileStats.columns;

				retVal->attributes[attributeName] = ParseValue(sourceIter);

				SkipWhiteSpace(sourceIter);
			} while (*sourceIter == ',' && ++sourceIter != jsonSource.cend());

			if (*sourceIter != '}') {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Did not find object close '}' at " + GetStandardErrorMessage(sourceIter));
			}

			++fileStats.objects;
			++fileStats.columns;
			++sourceIter;
			return retVal;
		}

		std::shared_ptr<JsonArray> ParseArray(std::string::const_iterator& sourceIter) {
			std::shared_ptr<JsonArray> retVal = std::make_shared<JsonArray>();
			++sourceIter;
			SkipWhiteSpace(sourceIter);

			if (*sourceIter == ']') {
				// Empty Array
				++fileStats.arrays;
				++fileStats.columns;
				++sourceIter;
				return retVal;
			}

			do {
				retVal->values.emplace_back(ParseValue(sourceIter));

				SkipWhiteSpace(sourceIter);
			} while (*sourceIter == ',' && ++sourceIter != jsonSource.cend());

			if (*sourceIter != ']') {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Did not find array close ']' at " + GetStandardErrorMessage(sourceIter));
			}

			++fileStats.arrays;
			++fileStats.columns;
			++sourceIter;

			return retVal;
		}

		std::shared_ptr<JsonElement> ParseValue(std::string::const_iterator& sourceIter) {
			SkipWhiteSpace(sourceIter);
			switch (*sourceIter) {
			case '{':
				return ParseObject(sourceIter);
				break;
			case '[':
				return ParseArray(sourceIter);
				break;
			case '"':
				return ParseString(sourceIter);
				break;
			case 't':
			case 'T':
				return ParseTrue(sourceIter);
				break;
			case 'f':
			case 'F':
				return ParseFalse(sourceIter);
				break;
			case 'n':
			case 'N':
				return ParseNull(sourceIter);
				break;
			case 'E':
			case 'e':
			case '+':
			case '-':
				return ParseNumber(sourceIter);
				break;
			default:
				if (isdigit(static_cast<unsigned char>(*sourceIter))) {
					return ParseNumber(sourceIter);
				}
				else {
					throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected character at " + GetStandardErrorMessage(sourceIter));
				}
			}
		}

		void ParseJson() {
			std::string::const_iterator sourceIter = jsonSource.cbegin();
			if (sourceIter == jsonSource.cend()) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Failed to read from file " + GetStandardErrorMessage(sourceIter));
			}

			SkipWhiteSpace(sourceIter);

			switch (*sourceIter) {
			case '{':
				++fileStats.columns;
				rootNode = ParseObject(sourceIter);
				break;
			case '[':
				++fileStats.columns;
				rootNode = ParseArray(sourceIter);
				break;
			default:
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Did not find '{' or '[' as first character in file.");
			}

			while (sourceIter != jsonSource.cend() && isspace(*sourceIter)) {
				fileStats.Increment(*sourceIter);
				++sourceIter;
			}

			if (sourceIter != jsonSource.cend()) {
				throw std::runtime_error(FILE_FUNCTION_LINE + ": Unexpected character encountered after end of validation.");
			}
		}
	};
};
#undef FILE_FUNCTION_LINE