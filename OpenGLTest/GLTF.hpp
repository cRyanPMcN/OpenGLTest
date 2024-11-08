#pragma once
#include "JsonParse.hpp"
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <cstdlib>

#define FILE_FUNCTION_LINE std::string(__FILE__) + ":" + std::string(__FUNCTION__) + '@' + std::to_string(__LINE__)

namespace GLTF {
	using type_json_element = std::shared_ptr<JsonParse::JsonElement>;
	using type_json_object = std::shared_ptr<JsonParse::JsonObject>;
	using type_json_array = std::shared_ptr<JsonParse::JsonArray>;
	using type_json_string = std::shared_ptr<JsonParse::JsonString>;
	using type_json_boolean = std::shared_ptr<JsonParse::JsonBoolean>;
	using type_json_integer = std::shared_ptr<JsonParse::JsonInteger>;
	using type_json_number = std::shared_ptr<JsonParse::JsonNumber>;
	using type_json_null = std::shared_ptr<JsonParse::JsonNull>;

	struct GltfException : std::runtime_error {
		const type_json_object sourceObject;
		GltfException(const type_json_object object, const std::string& message) : sourceObject(object), std::runtime_error(message) {

		}

		GltfException(const type_json_object object, const char* message) : sourceObject(object), std::runtime_error(message) {
			 
		}
	};

	struct GltfMissingElement : public GltfException {
		GltfMissingElement(const type_json_object object, const std::string& message) : GltfException(object, message) {

		}

		GltfMissingElement(const type_json_object object, const char* message) : GltfException(object, message) {

		}
	};

	struct GltfTypeMismatch : GltfException {
		GltfTypeMismatch(const type_json_object object, const std::string& messagePreamble, JsonParse::Type expectedType, JsonParse::Type foundType) : GltfException(object, messagePreamble) {

		}

		GltfTypeMismatch(const type_json_object object, const std::string& message) : GltfException(object, message) {

		}

		GltfTypeMismatch(const type_json_object object, const char* message) : GltfException(object, message) {

		}
	};

	struct GltfAccessorSparseCountMismatch : GltfException {
		GltfAccessorSparseCountMismatch(const type_json_object object, const std::string& message) : GltfException(object, message) {

		}

		GltfAccessorSparseCountMismatch(const type_json_object object, const char* message) : GltfException(object, message) {

		}
	};

	struct GltfArraySizeMismatch : GltfException {
		GltfArraySizeMismatch(const type_json_object object, const std::string& message) : GltfException(object, message) {

		}

		GltfArraySizeMismatch(const type_json_object object, const char* message) : GltfException(object, message) {

		}
	};

	// Type-Definitions

	using number_type = JsonParse::JsonNumber::value_type;
	using index_type = size_t;
	using integer_type = JsonParse::JsonInteger::value_type;

	namespace Enumerations {
		enum BufferViewTarget : integer_type {
			ARRAY_BUFFER = 34962,
			ELEMENT_ARRAY_BUFFER = 34963
		};

		enum ComponentType : integer_type {
			Uninitialized = 0,
			Byte = 5120,
			Unsigned_Byte = 5121,
			Short = 5122,
			Unsigned_Short = 5123,
			Int = 5124,		// Not defined in GLTF
			Unsigned_Int = 5125,
			Float = 5126
		};

		enum SamplerFilter : integer_type {
			NEAREST = 9728,
			LINEAR = 9729,
			NEAREST_MIPMAP_NEAREST = 9984,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR,
		};

		enum SamplerWrap : integer_type {
			CLAMP_TO_EDGE = 33071,
			MIRRORED_REPEAT = 33648,
			REPEAT = 10497,
		};
	}

	namespace Constants {
		// GLTFProperty
		const static std::string EXTENSIONS = "extensions";
		const static std::string EXTRAS = "extras";

		// Common Constants
		const static std::string URI = "uri";
		const static std::string NAME = "name";
		const static std::string COUNT = "count";
		const static std::string BUFFER_VIEW = "bufferView";
		const static std::string BYTE_OFFSET = "byteOffset";
		const static std::string COMPONENT_TYPE = "componentType";
		const static std::string SAMPLERS = "samplers"; // Animation/GLTFDoc
		const static std::string NODES = "nodes"; // Scene/GLTFDoc

		// Asset
		const static std::string COPYRIGHT = "copyright";
		const static std::string GENERATOR = "generator";
		const static std::string VERSION = "version";
		const static std::string MIN_VERSION = "minVersion";

		// Accessor
		const static std::string NORMALIZED = "normalized";
		const static std::string TYPE = "type";
		const static std::string MAX = "max";
		const static std::string MIN = "min";
		const static std::string SPARSE = "sparse";
		const static std::string INDICES = "indices";
		const static std::string VALUES = "values";
		const static std::string SCALAR = "SCALAR";
		const static std::string VEC2 = "VEC2";
		const static std::string VEC3 = "VEC3";
		const static std::string VEC4 = "VEC4";
		const static std::string MAT2 = "MAT2";
		const static std::string MAT3 = "MAT3";
		const static std::string MAT4 = "MAT4";

		// Animation
		const static std::string CHANNELS = "channels";
		const static std::string SAMPLER = "sampler";
		const static std::string ANIMATION_SAMPLERS = "animation_samplers"; // Used in validator to separate the GLTF::SAMPLERS from GLTF::ANIMATION::SAMPLERS
		const static std::string TARGET = "target";
		const static std::string NODE = "node";
		const static std::string PATH = "path";
		const static std::string INPUT = "input";
		const static std::string INTERPOLATION = "interpolation";
		const static std::string INTERPOLATION_LINEAR = "LINEAR";
		const static std::string INTERPOLATION_ROTATION = "ROTATION";
		const static std::string DEFAULT_INTERPOLATION = INTERPOLATION_LINEAR;
		const static std::string OUTPUT = "output";

		// Buffer
		const static std::string BYTE_LENGTH = "byteLength";
		const static std::string STREAM_DATA("data:");
		const static std::string STREAM_CHECK_OCTECT("application/octet-stream;");
		const static std::string STREAM_CHECK_GLTF("application/gltf-buffer;");
		const static std::string STREAM_SEPERATOR("base64,");

		// BufferView
		const static std::string BUFFER = "buffer";
		const static std::string BYTE_STRIDE = "byteStride";

		// Camera
		const static std::string ORTHOGRAPHIC = "orthographic";
		const static std::string PERSPECTIVE = "perspective";
		const static std::string XMAG = "xmag";
		const static std::string YMAG = "ymag";
		const static std::string ZFAR = "zfar";
		const static std::string ZNEAR = "znear";
		const static std::string ASPECT_RATIO = "aspectRatio";
		const static std::string YFOV = "yfov";

		// Image
		const static std::string MIME_TYPE = "mimeType";
		const static std::string MIME_IMAGE_JPEG = "image/jpeg";
		const static std::string MIME_IMAGE_PNG = "image/png";

		// Material
		const static std::string ALPHA_MODE = "alphaMode";
		const static std::string DEFAULT_ALPHA_MODE = "OPAQUE";
		const static std::string ALPHA_CUTOFF = "alphaCutoff";
		const static std::string PBR_METALLIC_ROUGHNESS = "pbrMetallicRoughness";
		const static std::string BASE_COLOR_FACTOR = "baseColorFactor";
		const static std::string BASE_COLOR_TEXTURE = "baseColorTexture";
		const static std::string METALLIC_FACTOR = "metallicFactor";
		const static std::string ROUGHNESS_FACTOR = "roughnessFactor";
		const static std::string METALLIC_ROUGHNESS_TEXTURE = "metallicRoughnessTexture";
		const static std::string NORMAL_TEXTURE = "normalTexture";
		const static std::string OCCLUSION_TEXTURE = "occlusionTexture";
		const static std::string EMISSIVE_TEXTURE = "emissiveTexture";
		const static std::string EMISSIVE_FACTOR = "emissiveFactor";
		const static std::string STRENGTH = "strength";
		const static std::string DOUBLE_SIDED = "doubleSided";

		// Mesh
		const static std::string PRIMITIVES = "primitives";
		const static std::string WEIGHTS = "weights";
		const static std::string ATTRIBUTES = "attributes";
		const static std::string MATERIAL = "material";
		const static std::string MODE = "mode";
		const static std::string TARGETS = "targets";

		// Node
		const static std::string CAMERA = "camera";
		const static std::string SKIN = "skin";
		const static std::string MESH = "mesh";
		const static std::string CHILDREN = "children";
		const static std::string MATRIX = "matrix";
		const static std::string ROTATION = "rotation";
		const static std::string TRANSLATION = "translation";

		// Sampler
		const static std::string MAG_FILTER = "magFilter";
		const static std::string MIN_FILTER = "minFilter";
		const static std::string WRAP_S = "wrapS";
		const static std::string WRAP_T = "wrapT";

		// TextureInfo
		const static std::string INDEX = "index";
		const static std::string TEX_COORD = "texCoord";
		const static std::string SCALE = "scale";

		// Skin
		const static std::string INVERSE_BIND_MATRICES = "inverseBindMatrices";
		const static std::string SKELETON = "skeleton";
		const static std::string JOINTS = "joints";

		// Texture
		const static std::string SOURCE = "source";

		// GLTFDoc
		const static std::string ACCESSORS = "accessors";
		const static std::string ANIMATIONS = "animations";
		const static std::string ASSET = "asset";
		const static std::string BUFFERS = "buffers";
		const static std::string BUFFER_VIEWS = "bufferViews";
		const static std::string CAMERAS = "cameras";
		const static std::string IMAGES = "images";
		const static std::string MATERIALS = "materials";
		const static std::string MESHES = "meshes";
		const static std::string SCENE = "scene";
		const static index_type SCENE_DEFAULT_VALUE = -1;
		const static std::string SCENES = "scenes";
		const static std::string SKINS = "skins";
		const static std::string TEXTURES = "textures";
		const static std::string EXTENSIONS_USED = "extensionsUsed";
		const static std::string EXTENSIONS_REQUIRED = "extensionsRequired";
	}

	inline std::string JsonParse_Type_To_String(JsonParse::Type const type) {
		switch (type) {
		case JsonParse::Type::Object:
			return "Object";
		case JsonParse::Type::Array:
			return "Array";
		case JsonParse::Type::String:
			return "String";
		case JsonParse::Type::Boolean:
			return "Boolean";
		case JsonParse::Type::Integer:
			return "Integer";
		case JsonParse::Type::Number:
			return "Number";
		case JsonParse::Type::Null:
			return "Null";
		default:
			throw std::invalid_argument(FILE_FUNCTION_LINE + ": Unknown type.");
		}
	}

#define CALLBACK_ARGS(objectName) std::string const& messagePreamble, type_json_object const& objectName
#define CALLBACK_INTEGER_ARGS(objectName) CALLBACK_ARGS(objectName), type_json_integer const& element
#define CALLBACK_NUMBER_ARGS(objectName) CALLBACK_ARGS(objectName), type_json_number const& element
#define CALLBACK_STRING_ARGS(objectName) CALLBACK_ARGS(objectName), type_json_string const& element
#define CALLBACK_ARRAY_ARGS CALLBACK_ARGS(object), type_json_array const& jsonArray

	struct Validator {
		using callback_integer = void(Validator::*)(std::string const&, type_json_object const&, type_json_integer const&);
		using callback_number = void(Validator::*)(std::string const&, type_json_object const&, type_json_number const&);
		using callback_string = void(Validator::*)(std::string const&, type_json_object const&, type_json_string const&);
		template <class..._ArgsTy>
		using callback_array = void(Validator::*)(std::string const&, type_json_object const&, type_json_array const&, _ArgsTy...);

		class ManageBreadCrumb {
			Validator& _validator;
		public:
			ManageBreadCrumb(Validator& validator, std::string const& elementName) : _validator(validator) {
				_validator.nameBreadCrumbs.push_back(elementName);
			}

			~ManageBreadCrumb() {
				_validator.nameBreadCrumbs.pop_back();
			}
		};

		struct GLTFError {
			type_json_object parent;
			type_json_element source;
			std::string message;

			GLTFError(type_json_object const& parentObject, type_json_element sourceElement, std::string errorMessage) :
				parent(parentObject), source(sourceElement), message(errorMessage) {

			}
		};

		struct AccessorInfo {
			std::string accessorType = "";
			Enumerations::ComponentType componentType = Enumerations::ComponentType::Uninitialized;
			integer_type count = 0;
		};

		std::map<std::string, void(*)(Validator&, type_json_object const&)> extensionHandlers;

		std::vector<GLTFError> errors;
		std::vector<GLTFError> warnings;
		std::set<std::string> extensionsInFile;
		std::map<std::string, JsonParse::JsonInteger::value_type> arraySizes;
		// Track size of joint array in each 'skin' element
		// Is reset after the 'skin' element is processed
		index_type sizeArrayJoints;
		// Used to track how many accessors reference a single bufferView
		std::map<index_type, integer_type> referencesAccessorToBufferView;
		std::vector<AccessorInfo> accessorsInfo;
		std::vector<AccessorInfo>::iterator accessorsIterator;
		// Stores Mesh.Weights.size() or Mesh.Primitives[0].Targets.size()
		std::vector<size_t> sizeMeshWeights;
		size_t expectedTargetSize;
		// Track the mesh.primitives[].attributes[] for checking primitive.targets[]
		// Is reset after each mesh.primitive is finished
		std::unordered_set<std::string> meshPrimitiveAttributes;
		size_t bufferViewIdx;
		// Used to generate error messages with a hierarchy to the error
		std::vector<std::string> nameBreadCrumbs;

		Validator(type_json_object const& rootObject) : bufferViewIdx(0), sizeArrayJoints(0) {
			Root(rootObject);
			referencesAccessorToBufferView.clear();
			accessorsInfo.clear();
			sizeMeshWeights.clear();
		}

		Validator(type_json_object const& rootObject, std::map<std::string, void(*)(Validator&, type_json_object const&)> _extensionHandlers) : extensionHandlers(_extensionHandlers), bufferViewIdx(0), sizeArrayJoints(0) {
			Root(rootObject);
			referencesAccessorToBufferView.clear();
			accessorsInfo.clear();
			sizeMeshWeights.clear();
		}

		std::string GetBreadCrumbs() const {
			std::string retVal;
			for (std::string const& crumb : nameBreadCrumbs) {
				retVal += crumb + "/";
			}
			return retVal;
		}

		std::string ErrorMessageStart(std::string const& messagePreamble) {
			return messagePreamble + ": " + GetBreadCrumbs();
		}

		std::string ErrorMessageValue(std::string const& messagePreamble, type_json_string const& element) {
			return ErrorMessageStart(messagePreamble) + " value:" + element->value;
		}

		std::string ErrorMessageValue(std::string const& messagePreamble, type_json_integer const& element) {
			return ErrorMessageStart(messagePreamble) + " value:" + std::to_string(element->value);
		}

		std::string ErrorMessageValue(std::string const& messagePreamble, type_json_number const& element) {
			return ErrorMessageStart(messagePreamble) + " value:" + std::to_string(element->value);
		}

		std::string ErrorMissing(std::string const& messagePreamble) {
			return ErrorMessageStart(messagePreamble) + " is missing.";
		}

		std::string ErrorTypeMismatch(std::string const& messagePreamble, JsonParse::Type expected, JsonParse::Type found) {
			return ErrorMessageStart(messagePreamble) + " must be '" + JsonParse_Type_To_String(expected) + "' but is '" + JsonParse_Type_To_String(found) + "'.";
		}

		std::string ErrorTypeMismatchNumber(std::string const& messagePreamble, JsonParse::Type found) {
			return ErrorMessageStart(messagePreamble) + " must be 'number' or 'integer' but is '" + JsonParse_Type_To_String(found) + "'.";
		}

		std::string ErrorArrayTypeMismatch(std::string const& messagePreamble, size_t idx, JsonParse::Type expected, JsonParse::Type found) {
			return ErrorMessageStart(messagePreamble) + "@index:" + std::to_string(idx) + " must be '" +
				JsonParse_Type_To_String(expected) + "' but is '" + JsonParse_Type_To_String(found) + "'.";
		}

		std::string ErrorArrayTypeMismatchNumber(std::string const& messagePreamble, size_t idx, JsonParse::Type found) {
			return ErrorMessageStart(messagePreamble) + "@index:" + std::to_string(idx) + " must be 'number' or 'integer' but is '" + JsonParse_Type_To_String(found) + "'.";
		}

		std::string ErrorArraySize(std::string const& messagePreamble, size_t expectedSize) {
			return ErrorMessageStart(messagePreamble) + " array not of expected size: " + std::to_string(expectedSize) + ".";
		}

		std::string ErrorArrayUnique(std::string const& messagePreamble) {
			return ErrorMessageStart(messagePreamble) + " array must be unique values.";
		}

		size_t ArraySize(type_json_object const& object, std::string const& elementName) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::Type::Array) {
					return std::static_pointer_cast<JsonParse::JsonArray>(result)->values.size();
				}
			}

			// Array Size is only used for comparison
			// So a type-mismatch or a missing can be zero
			return size_t(0);
		}

		void ArrayGetSize(CALLBACK_ARRAY_ARGS, size_t& size) {
			size = jsonArray->values.size();
		}

		void ArrayOfExpectedSize(CALLBACK_ARRAY_ARGS, size_t expectedSize) {
			if (jsonArray->values.size() != expectedSize) {
				errors.push_back(GLTFError(object, jsonArray, ErrorArraySize(messagePreamble, expectedSize)));
			}
		}

		void ArrayUniqueIntegers(CALLBACK_ARRAY_ARGS) {
			std::set<integer_type> uniqueValues;
			for (type_json_element const& element : jsonArray->values) {
				if (element->type == JsonParse::Type::Integer) {
					integer_type value = std::static_pointer_cast<JsonParse::JsonInteger>(element)->value;
					if (uniqueValues.find(value) != uniqueValues.cend()) {
						errors.push_back(GLTFError(object, element, ErrorArrayUnique(messagePreamble)));
						// Stop checking as only one error message is needed
						break;
					}
					else {
						uniqueValues.insert(value);
					}
				}
			}
		}

		/// <summary>
		/// Calls callback on array-element, checks element exists and is an array
		/// </summary>
		/// <typeparam name="..._ArgsTy">Variadic argument to allow extra arguments for the callback</typeparam>
		/// <param name="messagePreamble">FILE_FUNCTION_LINE</param>
		/// <param name="object">Object to search for array</param>
		/// <param name="elementName">Element to search for</param>
		/// <param name="callback">Function to call</param>
		/// <param name="...args">Extra arguments</param>
		template <class..._ArgsTy>
		void Array(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_array<_ArgsTy...> callback, _ArgsTy...args) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result && result->type == JsonParse::Type::Array) {
				std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonArray>(result), args...);
			}
		}

		void ArrayOfObjects(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			void(Validator::* callback)(type_json_object const&) = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type != JsonParse::Type::Array) {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Array, result->type)));
				}
				else {
					type_json_array arr = std::static_pointer_cast<JsonParse::JsonArray>(result);
					for (size_t idx = 0; idx < arr->values.size(); ++idx) {
						if (arr->values[idx]->type != JsonParse::Type::Object) {
							errors.push_back(GLTFError(object, arr->values[idx], ErrorArrayTypeMismatch(messagePreamble, idx, JsonParse::Type::Object, arr->values[idx]->type)));
						}
						else {
							std::invoke(callback, *this, std::static_pointer_cast<JsonParse::JsonObject>(arr->values[idx]));
						}
					}
				}
			}
		}

		void ArrayOfNumbers(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_number callback = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type != JsonParse::Type::Array) {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Array, result->type)));
				}
				else {
					type_json_array arr = std::static_pointer_cast<JsonParse::JsonArray>(result);
					for (size_t idx = 0; idx < arr->values.size(); ++idx) {
						if (arr->values[idx]->type != JsonParse::Type::Number) {
							if ((*arr)[idx]->type != JsonParse::Type::Integer) {
								errors.push_back(GLTFError(object, (*arr)[idx], ErrorArrayTypeMismatchNumber(messagePreamble, idx, (*arr)[idx]->type)));
							}
							else {
								type_json_number converted = std::make_shared<JsonParse::JsonNumber>(static_cast<number_type>(std::static_pointer_cast<JsonParse::JsonInteger>((*arr)[idx])->value));
								arr->values[idx] = converted;
								if (callback) {
									std::invoke(callback, *this, messagePreamble, object, converted);
								}
							}
						}
						else {
							if (callback) {
								std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonNumber>((*arr)[idx]));
							}
						}
					}
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void ArrayOfIntegers(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_integer callback = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type != JsonParse::Type::Array) {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Array, result->type)));
				}
				else {
					type_json_array arr = std::static_pointer_cast<JsonParse::JsonArray>(result);
					for (size_t idx = 0; idx < arr->values.size(); ++idx) {
						if ((*arr)[idx]->type != JsonParse::Type::Integer) {
							errors.push_back(GLTFError(object, (*arr)[idx], ErrorArrayTypeMismatch(messagePreamble, idx, JsonParse::Type::Integer, (*arr)[idx]->type)));
						}
						else {
							if (callback) {
								std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonInteger>((*arr)[idx]));
							}
						}
					}
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void Object(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			void(Validator::* callback)(type_json_object const&) = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::Type::Object) {
					if (callback) {
						std::invoke(callback, *this, std::static_pointer_cast<JsonParse::JsonObject>(result));
					}
				}
				else {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Object, result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void String(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_string callback = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::JsonString::Class_Type()) {
					if (callback) {
						std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonString>(result));
					}
				}
				else {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::JsonString::Class_Type(), result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void Integer(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_integer callback = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::Type::Integer) {
					if (callback) {
						std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonInteger>(result));
					}
				}
				else {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Integer, result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void Index(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName, std::string const& arrayName, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::Type::Integer) {
					type_json_integer integer = std::static_pointer_cast<JsonParse::JsonInteger>(result);
					GreaterEqualZero(messagePreamble, object, integer);
					if (integer->value >= static_cast<integer_type>(arraySizes[arrayName])) {
						errors.push_back(GLTFError(object, result, ErrorMessageValue(messagePreamble, integer) + " must be >= 0, and < " + arrayName + ".size():" + std::to_string(arraySizes[arrayName]) + "."));
					}
				}
				else {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Integer, result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void Number(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName,
			callback_number callback = nullptr, bool required = false) {
			ManageBreadCrumb crumbs(*this, elementName);
			type_json_element result = object->Find(elementName);
			if (result) {
				if (result->type == JsonParse::Type::Number) {
					if (callback) {
						std::invoke(callback, *this, messagePreamble, object, std::static_pointer_cast<JsonParse::JsonNumber>(result));
					}
				}
				else if (result->type == JsonParse::Type::Integer) {
					type_json_number converted = std::make_shared<JsonParse::JsonNumber>(static_cast<number_type>(std::static_pointer_cast<JsonParse::JsonInteger>(result)->value));
					(*object)[elementName] = converted;
					if (callback) {
						std::invoke(callback, *this, messagePreamble, object, converted);
					}
				}
				else {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatchNumber(messagePreamble, result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void Boolean(std::string const& messagePreamble, type_json_object const& object, std::string const& elementName, bool required = false) {
			type_json_element result = object->Find(elementName);
			ManageBreadCrumb crumbs(*this, elementName);
			if (result) {
				if (result->type != JsonParse::Type::Boolean) {
					errors.push_back(GLTFError(object, result, ErrorTypeMismatch(messagePreamble, JsonParse::JsonBoolean::Class_Type(), result->type)));
				}
			}
			else {
				if (required) {
					errors.push_back(GLTFError(object, nullptr, ErrorMissing(messagePreamble)));
				}
			}
		}

		void GreaterEqualZero(CALLBACK_INTEGER_ARGS(object)) {
			if (element->value < 0) {
				errors.push_back(GLTFError(object, element, ErrorMessageValue(messagePreamble, element) + " must be >= 0."));
			}
		}

		void GreaterEqualOne(CALLBACK_INTEGER_ARGS(object)) {
			if (element->value < 1) {
				errors.push_back(GLTFError(object, element, ErrorMessageValue(messagePreamble, element) + " must be >= 1."));
			}
		}

		void GreaterEqualZero(CALLBACK_NUMBER_ARGS(object)) {
			if (element->value < 0.0) {
				errors.push_back(GLTFError(object, element, ErrorMessageValue(messagePreamble, element) + " must be >= 0."));
			}
		}

		void RangeZeroToOne(CALLBACK_NUMBER_ARGS(object)) {
			if (element->value < -1.0 || element->value > 1.0) {
				errors.push_back(GLTFError(object, element, ErrorMessageValue(messagePreamble, element) + "must be >= -1.0 && <= 1.0."));
			}
		}

		void Extensions(type_json_object const& extensions) {
			for (std::pair<std::string, type_json_element> const& extension : extensions->attributes) {
				this->extensionsInFile.insert(extension.first);
			}
		}

		void Property(std::string const& messagePreamble, type_json_object const& object) {
			Object(messagePreamble, object, Constants::EXTENSIONS, &Validator::Extensions);
			Object(messagePreamble, object, Constants::EXTRAS);
		}

		void RootProperty(std::string const& messagePreamble, type_json_object const& object) {
			Property(messagePreamble, object);
			String(messagePreamble, object, Constants::NAME, nullptr);
		}

		void AccessorBufferView(CALLBACK_INTEGER_ARGS(accessor)) {
			GreaterEqualZero(messagePreamble, accessor, element);
			if (element->value >= static_cast<integer_type>(arraySizes[Constants::BUFFER_VIEWS])) {
				errors.push_back(GLTFError(accessor, element, ErrorMessageValue(messagePreamble, element) + " must be >= 0, and < meshes.size():" + std::to_string(arraySizes[Constants::BUFFER_VIEWS]) + "."));
			}
			++(referencesAccessorToBufferView[element->value]);
		}

		void AccessorComponentType(CALLBACK_INTEGER_ARGS(accessor)) {
			switch (element->value) {
			case Enumerations::ComponentType::Byte:
			case Enumerations::ComponentType::Unsigned_Byte:
			case Enumerations::ComponentType::Short:
			case Enumerations::ComponentType::Unsigned_Short:
			case Enumerations::ComponentType::Unsigned_Int:
			case Enumerations::ComponentType::Float:
				break;
			default:
				errors.push_back(GLTFError(accessor, element, ErrorMessageValue(messagePreamble, element) + " must be " +
					std::to_string(Enumerations::ComponentType::Byte) + ", " + std::to_string(Enumerations::ComponentType::Unsigned_Byte) + ", " +
					std::to_string(Enumerations::ComponentType::Short) + ", " + std::to_string(Enumerations::ComponentType::Unsigned_Short) + ", " +
					std::to_string(Enumerations::ComponentType::Unsigned_Int) + ", or " + std::to_string(Enumerations::ComponentType::Float) + "."));
			}
			accessorsIterator->componentType = (Enumerations::ComponentType)element->value;
		}

		void MaxMinArray(CALLBACK_ARRAY_ARGS, size_t expectedSize) {
			if (jsonArray->values.size() != expectedSize) {
				errors.push_back(GLTFError(object, jsonArray, ErrorArraySize(messagePreamble, expectedSize)));
			}

			size_t idx = 0;
			for (size_t idx = 0; idx < jsonArray->values.size(); ++idx) {
				type_json_element const& value = jsonArray->values[idx];
				if (value->type != JsonParse::Type::Number && value->type != JsonParse::Type::Integer) {
					errors.push_back(GLTFError(object, jsonArray, ErrorArrayTypeMismatchNumber(messagePreamble, idx, value->type)));
				}
				++idx;
			}
		}

		void MaxMin(std::string const& messagePreamble, type_json_object const& accessor, std::string const& type) {
			size_t expectedSize;
			if (type == "SCALAR") {
				expectedSize = 1;
			}
			else if (type == "VEC2") {
				expectedSize = 2;
			}
			else if (type == "VEC3") {
				expectedSize = 3;
			}
			else if (type == "VEC4" || type == "MAT2") {
				expectedSize = 4;
			}
			else if (type == "MAT3") {
				expectedSize = 9;
			}
			else if (type == "MAT4") {
				expectedSize = 16;
			}
			else {
				// Final check just in case
				expectedSize = -1;
			}

			Array(messagePreamble, accessor, Constants::MAX, &Validator::MaxMinArray, expectedSize);
			Array(messagePreamble, accessor, Constants::MIN, &Validator::MaxMinArray, expectedSize);
		}

		void AccessorType(CALLBACK_STRING_ARGS(accessor)) {
			if (element->value != "SCALAR" && element->value != "VEC2" && element->value != "VEC3" && element->value != "VEC4" && element->value != "MAT2" && element->value != "MAT3" && element->value != "MAT4") {
				errors.push_back(GLTFError(accessor, element, ErrorMessageValue(messagePreamble, element) + " must be 'SCALAR', 'VEC2', 'VEC3', 'VEC4', 'MAT2', 'MAT3', or 'MAT4'."));
				ArrayOfNumbers(messagePreamble, accessor, Constants::MAX);
				ArrayOfNumbers(messagePreamble, accessor, Constants::MIN);
			}
			else {
				accessorsIterator->accessorType = element->value;
				MaxMin(messagePreamble, accessor, element->value);
			}
		}

		void SparseIndexComponentType(CALLBACK_INTEGER_ARGS(index)) {
			switch (element->value) {
			case Enumerations::ComponentType::Unsigned_Byte:
			case Enumerations::ComponentType::Unsigned_Short:
			case Enumerations::ComponentType::Unsigned_Int:
				break;
			default:
				errors.push_back(GLTFError(index, element, ErrorMessageValue(messagePreamble, element) + " must be "
					+ std::to_string(Enumerations::ComponentType::Unsigned_Byte) + ", " + std::to_string(Enumerations::ComponentType::Unsigned_Short) + ", or " + std::to_string(Enumerations::ComponentType::Unsigned_Int) + "."));
			}
		}

		void SparseIndex(type_json_object const& index) {
			Property(FILE_FUNCTION_LINE, index);
			Index(FILE_FUNCTION_LINE, index, Constants::BUFFER_VIEW, Constants::BUFFER_VIEWS, true);
			Integer(FILE_FUNCTION_LINE, index, Constants::BYTE_OFFSET, &Validator::GreaterEqualZero, false);
			Integer(FILE_FUNCTION_LINE, index, Constants::COMPONENT_TYPE, &Validator::SparseIndexComponentType, true);
		}

		void SparseValue(type_json_object const& value) {
			Property(FILE_FUNCTION_LINE, value);
			Index(FILE_FUNCTION_LINE, value, Constants::BUFFER_VIEW, Constants::BUFFER_VIEWS, true);
			Integer(FILE_FUNCTION_LINE, value, Constants::BYTE_OFFSET, &Validator::GreaterEqualZero, false);
		}

		void SparseCount(CALLBACK_INTEGER_ARGS(sparse)) {
			GreaterEqualOne(messagePreamble, sparse, element);
			Array(messagePreamble, sparse, Constants::INDICES, &Validator::ArrayOfExpectedSize, (size_t)element->value);
			Array(messagePreamble, sparse, Constants::VALUES, &Validator::ArrayOfExpectedSize, (size_t)element->value);
			ArrayOfObjects(messagePreamble, sparse, Constants::INDICES, &Validator::SparseIndex, true);
			ArrayOfObjects(messagePreamble, sparse, Constants::VALUES, &Validator::SparseValue, true);
		}

		void Sparse(type_json_object const& sparse) {
			Property(FILE_FUNCTION_LINE, sparse);
			Integer(FILE_FUNCTION_LINE, sparse, Constants::COUNT, &Validator::SparseCount, true);
		}

		void AccessorCount(CALLBACK_INTEGER_ARGS(accessor)) {
			GreaterEqualOne(messagePreamble, accessor, element);
			accessorsIterator->count = element->value;
		}

		void Accessor(type_json_object const& accessor) {
			RootProperty(FILE_FUNCTION_LINE, accessor);
			Integer(FILE_FUNCTION_LINE, accessor, Constants::BUFFER_VIEW, &Validator::AccessorBufferView, false);
			Integer(FILE_FUNCTION_LINE, accessor, Constants::BYTE_OFFSET, &Validator::GreaterEqualZero, false);
			Integer(FILE_FUNCTION_LINE, accessor, Constants::COMPONENT_TYPE, &Validator::AccessorComponentType, true);
			Boolean(FILE_FUNCTION_LINE, accessor, Constants::NORMALIZED);
			Integer(FILE_FUNCTION_LINE, accessor, Constants::COUNT, &Validator::GreaterEqualOne, true);
			String(FILE_FUNCTION_LINE, accessor, Constants::TYPE, &Validator::AccessorType, true);
			// Max and Min are handled in AccessorType
			Object(FILE_FUNCTION_LINE, accessor, Constants::SPARSE, &Validator::Sparse);
			++accessorsIterator;
		}

		void AnimationChannelTargetPath(CALLBACK_STRING_ARGS(target)) {
			if (element->value != Constants::TRANSLATION && element->value != Constants::ROTATION && element->value != Constants::SCALE && element->value != Constants::WEIGHTS) {
				errors.push_back(GLTFError(target, element, ErrorMessageValue(messagePreamble, element) + " must be '" + Constants::TRANSLATION + "', '" + Constants::ROTATION + "', '" + Constants::SCALE+ "', or '" + Constants::WEIGHTS + "'."));
			}
		}

		void AnimationChannelTarget(type_json_object const& target) {
			Property(FILE_FUNCTION_LINE, target);
			Index(FILE_FUNCTION_LINE, target, Constants::NODE, Constants::NODES);
			String(FILE_FUNCTION_LINE, target, Constants::PATH, &Validator::AnimationChannelTargetPath, true);
		}

		void AnimationChannel(type_json_object const& channel) {
			Property(FILE_FUNCTION_LINE, channel);
			Index(FILE_FUNCTION_LINE, channel, Constants::SAMPLER, Constants::ANIMATION_SAMPLERS, true);
			Object(FILE_FUNCTION_LINE, channel, Constants::TARGET, &Validator::AnimationChannelTarget, true);
		}

		void AnimationSamplerInterpolation(CALLBACK_STRING_ARGS(sampler)) {
			if (element->value != "LINEAR" && element->value != "STEP" && element->value != "CUBICSPLINE") {
				errors.push_back(GLTFError(sampler, element, ErrorMessageValue(messagePreamble, element) + " must be 'LINEAR', 'STEP', or 'CUBICSPLINE'."));
			}
		}

		void AnimationSampler(type_json_object const& sampler) {
			Property(FILE_FUNCTION_LINE, sampler);
			Index(FILE_FUNCTION_LINE, sampler, Constants::INPUT, Constants::ACCESSORS, true);
			String(FILE_FUNCTION_LINE, sampler, Constants::INTERPOLATION, &Validator::AnimationSamplerInterpolation, false);
			Index(FILE_FUNCTION_LINE, sampler, Constants::OUTPUT, Constants::ACCESSORS, true);
		}

		void Animation(type_json_object const& animation) {
			RootProperty(FILE_FUNCTION_LINE, animation);
			arraySizes[Constants::ANIMATION_SAMPLERS] = ArraySize(animation, Constants::SAMPLERS);
			ArrayOfObjects(FILE_FUNCTION_LINE, animation, Constants::CHANNELS, &Validator::AnimationChannel, true);
			ArrayOfObjects(FILE_FUNCTION_LINE, animation, Constants::SAMPLERS, &Validator::AnimationSampler, true);
		}


		void AssetVersion(CALLBACK_STRING_ARGS(asset)) {
			//std::regex reg("^[0-9]+.[0-9]+$"));
			if (!std::regex_match(element->value, std::regex("^\\d+\\.\\d+$"))) {
				errors.push_back(GLTFError(asset, element, ErrorMessageValue(messagePreamble, element) + " must match the pattern <Major>.<Minor>."));
			}
		}

		void Asset(type_json_object const& asset) {
			Property(FILE_FUNCTION_LINE, asset);
			String(FILE_FUNCTION_LINE, asset, Constants::VERSION, &Validator::AssetVersion, true);
			String(FILE_FUNCTION_LINE, asset, Constants::MIN_VERSION, &Validator::AssetVersion);
			String(FILE_FUNCTION_LINE, asset, Constants::COPYRIGHT);
			String(FILE_FUNCTION_LINE, asset, Constants::GENERATOR);
		}


		void Buffer(type_json_object const& buffer) {
			RootProperty(FILE_FUNCTION_LINE, buffer);
			String(FILE_FUNCTION_LINE, buffer, Constants::URI);
			Integer(FILE_FUNCTION_LINE, buffer, Constants::BYTE_LENGTH, &Validator::GreaterEqualOne, true);
		}

		void BufferViewByteStride(CALLBACK_INTEGER_ARGS(bufferView)) {
			if (!(element->value >= 4 || element->value <= 252)) {
				errors.push_back(GLTFError(bufferView, element, ErrorMessageValue(messagePreamble, element) + " must be of the range [4, 252]."));
			}
		}

		void BufferViewTarget(CALLBACK_INTEGER_ARGS(bufferView)) {
			if (element->value != Enumerations::BufferViewTarget::ARRAY_BUFFER && element->value != Enumerations::BufferViewTarget::ELEMENT_ARRAY_BUFFER) {
				errors.push_back(GLTFError(bufferView, element, ErrorMessageValue(messagePreamble, element) + " must be " +
					std::to_string(Enumerations::BufferViewTarget::ARRAY_BUFFER) + ", or " +
					std::to_string(Enumerations::BufferViewTarget::ELEMENT_ARRAY_BUFFER) + "."));
			}
		}

		void BufferView(type_json_object const& bufferView) {
			RootProperty(FILE_FUNCTION_LINE, bufferView);
			Index(FILE_FUNCTION_LINE, bufferView, Constants::BUFFER, Constants::BUFFERS, true);
			Integer(FILE_FUNCTION_LINE, bufferView, Constants::BYTE_OFFSET, &Validator::GreaterEqualZero);
			Integer(FILE_FUNCTION_LINE, bufferView, Constants::BYTE_LENGTH, &Validator::GreaterEqualOne, true);
			// Byte_Stride is required if two or more Accessors reference this bufferView
			Integer(FILE_FUNCTION_LINE, bufferView, Constants::BYTE_STRIDE, &Validator::BufferViewByteStride, referencesAccessorToBufferView[bufferViewIdx] >= 2);
			Integer(FILE_FUNCTION_LINE, bufferView, Constants::TARGET, &Validator::BufferViewTarget);
			++bufferViewIdx;
		}

		void CameraOrthographic(type_json_object const& orthographic) {
			Property(FILE_FUNCTION_LINE, orthographic);
			Number(FILE_FUNCTION_LINE, orthographic, Constants::XMAG, nullptr, true);
			Number(FILE_FUNCTION_LINE, orthographic, Constants::YMAG, nullptr, true);
			Number(FILE_FUNCTION_LINE, orthographic, Constants::ZFAR, &Validator::GreaterEqualZero, true);
			Number(FILE_FUNCTION_LINE, orthographic, Constants::ZNEAR, &Validator::GreaterEqualZero, true);
		}

		void CameraPerspective(type_json_object const& perspective) {
			Property(FILE_FUNCTION_LINE, perspective);
			Number(FILE_FUNCTION_LINE, perspective, Constants::ASPECT_RATIO, &Validator::GreaterEqualZero);
			Number(FILE_FUNCTION_LINE, perspective, Constants::YMAG, &Validator::GreaterEqualZero);
			Number(FILE_FUNCTION_LINE, perspective, Constants::ZFAR, &Validator::GreaterEqualZero, true);
			Number(FILE_FUNCTION_LINE, perspective, Constants::ZNEAR, &Validator::GreaterEqualZero, true);
		}

		void CameraType(CALLBACK_STRING_ARGS(camera)) {
			if (element->value == Constants::ORTHOGRAPHIC) {
				ManageBreadCrumb crumbs(*this, Constants::ORTHOGRAPHIC);
				if (!camera->Find(Constants::ORTHOGRAPHIC)) {
					errors.push_back(GLTFError(camera, nullptr, ErrorMessageStart(messagePreamble) + " must be defined when camera/type is '" + Constants::ORTHOGRAPHIC + "'."));
				}
				if (camera->Find(Constants::PERSPECTIVE)) {
					errors.push_back(GLTFError(camera, camera->Find(Constants::PERSPECTIVE), ErrorMessageStart(messagePreamble) + " cannot be defined when camera/type is '" + Constants::ORTHOGRAPHIC + "'."));
				}
			}
			else if (element->value == Constants::PERSPECTIVE) {
				ManageBreadCrumb crumbs(*this, Constants::PERSPECTIVE);
				if (!camera->Find(Constants::PERSPECTIVE)) {
					errors.push_back(GLTFError(camera, nullptr, ErrorMessageStart(messagePreamble) + " must be defined when camera/type is '" + Constants::PERSPECTIVE + "'."));
				}
				if (camera->Find(Constants::ORTHOGRAPHIC)) {
					errors.push_back(GLTFError(camera, camera->Find(Constants::ORTHOGRAPHIC), ErrorMessageStart(messagePreamble) + " cannot be defined when camera/type is '" + Constants::PERSPECTIVE + "'."));
				}
			}
			else {
				errors.push_back(GLTFError(camera, element, ErrorMessageValue(messagePreamble, element) + " must be be " + Constants::PERSPECTIVE + ", or " + Constants::ORTHOGRAPHIC + "."));
			}
			// Check both objects as it cannot be known if the value of 'type' is correct
			// Or if an extra definition for the camera was left in the file
			Object(messagePreamble, camera, Constants::ORTHOGRAPHIC, &Validator::CameraOrthographic);
			Object(messagePreamble, camera, Constants::PERSPECTIVE, &Validator::CameraPerspective);
		}

		void Camera(type_json_object const& camera) {
			RootProperty(FILE_FUNCTION_LINE, camera);
			String(FILE_FUNCTION_LINE, camera, Constants::TYPE, &Validator::CameraType, true);
		}

		void ImageMimeType(CALLBACK_STRING_ARGS(image)) {
			if (element->value != Constants::MIME_IMAGE_JPEG && element->value != Constants::MIME_IMAGE_PNG) {
				errors.push_back(GLTFError(image, element, ErrorMessageValue(messagePreamble, element) + " must be '" + Constants::MIME_IMAGE_JPEG + "' or '" + Constants::MIME_IMAGE_PNG + "'."));
			}
		}

		void Image(type_json_object const& image) {
			RootProperty(FILE_FUNCTION_LINE, image);
			type_json_element uri = image->Find(Constants::URI), bufferView = image->Find(Constants::BUFFER_VIEW);

			if (uri) {
				if (bufferView) {
					errors.push_back(GLTFError(image, image, ErrorMessageStart(FILE_FUNCTION_LINE) + " uri and bufferView cannot be defined together."));
				}
				else {
					String(FILE_FUNCTION_LINE, image, Constants::URI);
				}
			}
			else {
				if (!bufferView) {
					errors.push_back(GLTFError(image, image, ErrorMessageStart(FILE_FUNCTION_LINE) + " uri or bufferView must be defined."));
				}
				else {
					Index(FILE_FUNCTION_LINE, image, Constants::BUFFER_VIEW, Constants::BUFFER_VIEWS);
					String(FILE_FUNCTION_LINE, image, Constants::MIME_TYPE, &Validator::ImageMimeType, true);
				}
			}
		}

		void MaterialPBRMetallicRoughness(type_json_object const& pbrMetalRough) {
			Property(FILE_FUNCTION_LINE, pbrMetalRough);
			ArrayOfNumbers(FILE_FUNCTION_LINE, pbrMetalRough, Constants::BASE_COLOR_FACTOR, &Validator::RangeZeroToOne);
			Array(FILE_FUNCTION_LINE, pbrMetalRough, Constants::BASE_COLOR_FACTOR, &Validator::ArrayOfExpectedSize, size_t(4));
			Object(FILE_FUNCTION_LINE, pbrMetalRough, Constants::BASE_COLOR_TEXTURE, &Validator::TextureInfo);
			Number(FILE_FUNCTION_LINE, pbrMetalRough, Constants::METALLIC_FACTOR, &Validator::RangeZeroToOne);
			Number(FILE_FUNCTION_LINE, pbrMetalRough, Constants::ROUGHNESS_FACTOR, &Validator::RangeZeroToOne);
			Object(FILE_FUNCTION_LINE, pbrMetalRough, Constants::METALLIC_ROUGHNESS_TEXTURE, &Validator::TextureInfo);
		}

		void MaterialNormalTexture(type_json_object const& normalTexture) {
			// Better for error messages to just copy the implementation of TextureInfo here
			Property(FILE_FUNCTION_LINE, normalTexture);
			Index(FILE_FUNCTION_LINE, normalTexture, Constants::INDEX, Constants::IMAGES, true);
			Integer(FILE_FUNCTION_LINE, normalTexture, Constants::TEX_COORD, &Validator::GreaterEqualZero);
			Number(FILE_FUNCTION_LINE, normalTexture, Constants::SCALE);
		}

		void MaterialOcclusionTexture(type_json_object const& occlussionTexture) {
			// Better for error messages to just copy the implementation of TextureInfo here
			Property(FILE_FUNCTION_LINE, occlussionTexture);
			Index(FILE_FUNCTION_LINE, occlussionTexture, Constants::INDEX, Constants::IMAGES, true);
			Integer(FILE_FUNCTION_LINE, occlussionTexture, Constants::TEX_COORD, &Validator::GreaterEqualZero);
			Number(FILE_FUNCTION_LINE, occlussionTexture, Constants::STRENGTH, &Validator::RangeZeroToOne);
		}

		void MaterialAlphaMode(CALLBACK_STRING_ARGS(material)) {
			if (element->value != "OPAQUE" && element->value != "MASK" && element->value != "BLEND") {
				errors.push_back(GLTFError(material, element, ErrorMessageValue(messagePreamble, element) + " must be 'OPAQUE', 'MASK', or 'BLEND'."));
			}
		}

		void Material(type_json_object const& material) {
			RootProperty(FILE_FUNCTION_LINE, material);
			Object(FILE_FUNCTION_LINE, material, Constants::PBR_METALLIC_ROUGHNESS, &Validator::MaterialPBRMetallicRoughness);
			Object(FILE_FUNCTION_LINE, material, Constants::NORMAL_TEXTURE, &Validator::MaterialNormalTexture);
			Object(FILE_FUNCTION_LINE, material, Constants::OCCLUSION_TEXTURE, &Validator::MaterialOcclusionTexture);
			Object(FILE_FUNCTION_LINE, material, Constants::EMISSIVE_TEXTURE, &Validator::TextureInfo);
			ArrayOfNumbers(FILE_FUNCTION_LINE, material, Constants::EMISSIVE_FACTOR, &Validator::RangeZeroToOne);
			Array(FILE_FUNCTION_LINE, material, Constants::EMISSIVE_FACTOR, &Validator::ArrayOfExpectedSize, size_t(3));
			String(FILE_FUNCTION_LINE, material, Constants::ALPHA_MODE, &Validator::MaterialAlphaMode);
			Number(FILE_FUNCTION_LINE, material, Constants::ALPHA_CUTOFF, &Validator::GreaterEqualZero);
			Boolean(FILE_FUNCTION_LINE, material, Constants::DOUBLE_SIDED);
		}

		void MeshPrimitiveAttributes(type_json_object const& attributes) {
			for (auto begin = attributes->attributes.cbegin(); begin != attributes->attributes.cend(); ++begin) {
				if (begin->second->type != JsonParse::Type::Integer) {
					errors.push_back(GLTFError(attributes, begin->second, ErrorMessageStart(FILE_FUNCTION_LINE) +
						" attribute:" + begin->first + " is type:" + JsonParse_Type_To_String(begin->second->type) + " must be an integer."));
				}
			}
		}

		void MeshPrimitiveIndices(CALLBACK_INTEGER_ARGS(primitive)) {
			Validator::GreaterEqualZero(messagePreamble, primitive, element);
			if (accessorsInfo[element->value].accessorType != "SCALAR") {
				errors.push_back(GLTFError(primitive, element, ErrorMessageStart(messagePreamble) + " accessorIndex:" + std::to_string(element->value) +
					"accessor.accessorType:" + accessorsInfo[element->value].accessorType + " must be 'SCALAR'."));
			}

			Enumerations::ComponentType& componentType = accessorsInfo[element->value].componentType;
			switch (componentType) {
			case Enumerations::ComponentType::Unsigned_Byte:
			case Enumerations::ComponentType::Unsigned_Short:
			case Enumerations::ComponentType::Unsigned_Int:
				break;
			default:
				errors.push_back(GLTFError(primitive, element, ErrorMessageStart(messagePreamble) + " accessorIndex:" + std::to_string(element->value) +
					"accessor.componentType:" + std::to_string(componentType) + " must be " + std::to_string(Enumerations::ComponentType::Unsigned_Byte) +
					+", " + std::to_string(Enumerations::ComponentType::Unsigned_Short) +
					+", or " + std::to_string(Enumerations::ComponentType::Unsigned_Int) + "."));
			}
		}

		void MeshPrimitiveMode(CALLBACK_INTEGER_ARGS(primitive)) {
			if (element->value < 0 || element->value > 6) {
				errors.push_back(GLTFError(primitive, element, ErrorMessageValue(messagePreamble, element) + " must be 0, 1, 2, 3, 4, 5, or 6."));
			}
		}

		void MeshPrimitive(type_json_object const& primitive) {
			Property(FILE_FUNCTION_LINE, primitive);
			Object(FILE_FUNCTION_LINE, primitive, Constants::ATTRIBUTES, &Validator::MeshPrimitiveAttributes, true);
			Integer(FILE_FUNCTION_LINE, primitive, Constants::INDICES, &Validator::GreaterEqualZero);
			Integer(FILE_FUNCTION_LINE, primitive, Constants::MATERIAL, &Validator::GreaterEqualZero);
			if (ArraySize(primitive, Constants::TARGETS) != sizeMeshWeights.back()) {
				errors.push_back(GLTFError(primitive, primitive->Find(Constants::TARGETS), ErrorMessageStart(FILE_FUNCTION_LINE) + " targets.size() must be equal to mesh.weights.size():" + std::to_string(sizeMeshWeights.back())));
			}
			ArraySize(primitive, Constants::TARGETS);
			ArrayOfObjects(FILE_FUNCTION_LINE, primitive, Constants::TARGETS);
		}

		void Mesh(type_json_object const& mesh) {
			RootProperty(FILE_FUNCTION_LINE, mesh);
			sizeMeshWeights.emplace_back(ArraySize(mesh, Constants::WEIGHTS));
			ArrayOfNumbers(FILE_FUNCTION_LINE, mesh, Constants::WEIGHTS);
			ArrayOfObjects(FILE_FUNCTION_LINE, mesh, Constants::PRIMITIVES, &Validator::MeshPrimitive, true);
		}

		void NodeMesh(CALLBACK_INTEGER_ARGS(node)) {
			if (element->value < 0 && element->value > static_cast<integer_type>(arraySizes[Constants::MESHES])) {
				errors.push_back(GLTFError(node, element, ErrorMessageValue(messagePreamble, element) + " must be >= 0, and < meshes.size():" + std::to_string(arraySizes[Constants::MESHES]) + "."));
			}
		}

		void Node(type_json_object const& node) {
			RootProperty(FILE_FUNCTION_LINE, node);
			Index(FILE_FUNCTION_LINE, node, Constants::CAMERA, Constants::CAMERAS);
			ArrayOfIntegers(FILE_FUNCTION_LINE, node, Constants::CHILDREN, &Validator::GreaterEqualZero);
			Array(FILE_FUNCTION_LINE, node, Constants::CHILDREN, &Validator::ArrayUniqueIntegers);
			Index(FILE_FUNCTION_LINE, node, Constants::SKIN, Constants::SKINS);
			ArrayOfNumbers(FILE_FUNCTION_LINE, node, Constants::MATRIX);
			Array(FILE_FUNCTION_LINE, node, Constants::MATRIX, &Validator::ArrayOfExpectedSize, size_t(16));

			expectedTargetSize = ArraySize(node, Constants::WEIGHTS);
			// Index function is more accurate but Index does not have a function pointer argument
			Integer(FILE_FUNCTION_LINE, node, Constants::MESH, &Validator::NodeMesh);
			expectedTargetSize = 0;

			ArrayOfNumbers(FILE_FUNCTION_LINE, node, Constants::ROTATION, &Validator::RangeZeroToOne);
			Array(FILE_FUNCTION_LINE, node, Constants::ROTATION, &Validator::ArrayOfExpectedSize, size_t(4));
			ArrayOfNumbers(FILE_FUNCTION_LINE, node, Constants::SCALE);
			Array(FILE_FUNCTION_LINE, node, Constants::SCALE, &Validator::ArrayOfExpectedSize, size_t(3));
			ArrayOfNumbers(FILE_FUNCTION_LINE, node, Constants::TRANSLATION);
			Array(FILE_FUNCTION_LINE, node, Constants::ROTATION, &Validator::ArrayOfExpectedSize, size_t(3));

			ArrayOfNumbers(FILE_FUNCTION_LINE, node, Constants::WEIGHTS);
		}

		void SamplerMagFilter(CALLBACK_INTEGER_ARGS(sampler)) {
			switch (element->value) {
			case Enumerations::SamplerFilter::LINEAR:
			case Enumerations::SamplerFilter::NEAREST:
				break;
			default:
				errors.push_back(GLTFError(sampler, element, ErrorMessageStart(messagePreamble) +
					" value:" + std::to_string(element->value) + " must be " + std::to_string(Enumerations::SamplerFilter::LINEAR) + ", or " + std::to_string(Enumerations::SamplerFilter::NEAREST) + "."));
			}
		}

		void SamplerMinFilter(CALLBACK_INTEGER_ARGS(sampler)) {
			switch (element->value) {
			case Enumerations::SamplerFilter::LINEAR:
			case Enumerations::SamplerFilter::NEAREST:
			case Enumerations::SamplerFilter::LINEAR_MIPMAP_LINEAR:
			case Enumerations::SamplerFilter::LINEAR_MIPMAP_NEAREST:
			case Enumerations::SamplerFilter::NEAREST_MIPMAP_LINEAR:
			case Enumerations::SamplerFilter::NEAREST_MIPMAP_NEAREST:
				break;
			default:
				errors.push_back(GLTFError(sampler, element, ErrorMessageStart(messagePreamble) +
					" value:" + std::to_string(element->value) + " must be " +
					std::to_string(Enumerations::SamplerFilter::LINEAR) + ", " + std::to_string(Enumerations::SamplerFilter::NEAREST) +
					std::to_string(Enumerations::SamplerFilter::LINEAR_MIPMAP_LINEAR) + ", " + std::to_string(Enumerations::SamplerFilter::LINEAR_MIPMAP_NEAREST) +
					std::to_string(Enumerations::SamplerFilter::NEAREST_MIPMAP_LINEAR) + ", or " + std::to_string(Enumerations::SamplerFilter::NEAREST_MIPMAP_NEAREST) + "."));
			}
		}

		void SamplerWrap(CALLBACK_INTEGER_ARGS(sampler)) {
			switch (element->value) {
			case Enumerations::SamplerWrap::CLAMP_TO_EDGE:
			case Enumerations::SamplerWrap::MIRRORED_REPEAT:
			case Enumerations::SamplerWrap::REPEAT:
				break;
			default:
				errors.push_back(GLTFError(sampler, element, ErrorMessageStart(messagePreamble) +
					" value:" + std::to_string(element->value) + " must be " +
					std::to_string(Enumerations::SamplerWrap::CLAMP_TO_EDGE) + ", " +
					std::to_string(Enumerations::SamplerWrap::MIRRORED_REPEAT) + ", or" +
					std::to_string(Enumerations::SamplerWrap::REPEAT) + "."));
			}
		}

		void Sampler(type_json_object const& sampler) {
			RootProperty(FILE_FUNCTION_LINE, sampler);
			Integer(FILE_FUNCTION_LINE, sampler, Constants::MAG_FILTER, &Validator::SamplerMagFilter);
			Integer(FILE_FUNCTION_LINE, sampler, Constants::MIN_FILTER, &Validator::SamplerMinFilter);
			Integer(FILE_FUNCTION_LINE, sampler, Constants::WRAP_S, &Validator::SamplerWrap);
			Integer(FILE_FUNCTION_LINE, sampler, Constants::WRAP_T, &Validator::SamplerWrap);
		}

		void SceneNode(CALLBACK_ARRAY_ARGS) {
			ArrayUniqueIntegers(messagePreamble, object, jsonArray);
			for (type_json_element const& element : jsonArray->values) {
				if (element->type != JsonParse::JsonInteger::Class_Type()) {
					errors.push_back(GLTFError(object, element, ErrorTypeMismatch(messagePreamble, JsonParse::Type::Integer, element->type)));
				}
				else {
					integer_type value = std::static_pointer_cast<JsonParse::JsonInteger>(element)->value;
					if (value < 0 || value >= static_cast<integer_type>(arraySizes[Constants::NODES])) {
						errors.push_back(GLTFError(object, element, 
							ErrorMessageValue(messagePreamble, 
								std::static_pointer_cast<JsonParse::JsonInteger>(element)) + " must be >= 0, and < " + Constants::NODES + ".size():" + std::to_string(arraySizes[Constants::NODES]) + "."));
					}
				}
			}
		}

		void Scene(type_json_object const& scene) {
			RootProperty(FILE_FUNCTION_LINE, scene);
			Array(FILE_FUNCTION_LINE, scene, Constants::NODES, &Validator::SceneNode);
		}

		void SkinBindMatrices(CALLBACK_INTEGER_ARGS(skin)) {
			GreaterEqualZero(messagePreamble, skin, element);
			if (element->value >= 0 && element->value < arraySizes[Constants::ACCESSORS]) {
				if (!(static_cast<decltype(sizeArrayJoints)>(accessorsInfo[element->value].count) >= sizeArrayJoints)) {
					errors.push_back(GLTFError(skin, element,
						ErrorMessageStart(messagePreamble) + ", accessorIndex:" + std::to_string(element->value) +
						", accessor.count:" + std::to_string(accessorsInfo[element->value].count) + ", must be >= joint.size():" + std::to_string(sizeArrayJoints) + "."));
				}
			}
			else {
				errors.push_back(GLTFError(skin, element, ErrorMessageStart(messagePreamble) + " accessorIndex:" +
					std::to_string(element->value) + " must be >= 0 and < accessors.size():" + std::to_string(accessorsInfo.size()) + "."));
			}
		}

		void Skin(type_json_object const& skin) {
			RootProperty(FILE_FUNCTION_LINE, skin);
			sizeArrayJoints = 0;
			ArrayOfIntegers(FILE_FUNCTION_LINE, skin, Constants::JOINTS, &Validator::GreaterEqualZero, true);
			sizeArrayJoints = ArraySize(skin, Constants::JOINTS);
			Array(FILE_FUNCTION_LINE, skin, Constants::JOINTS, &Validator::ArrayUniqueIntegers);
			Integer(FILE_FUNCTION_LINE, skin, Constants::INVERSE_BIND_MATRICES, &Validator::SkinBindMatrices);
			Index(FILE_FUNCTION_LINE, skin, Constants::SKELETON, Constants::NODES);
		}

		void Texture(type_json_object const& texture) {
			RootProperty(FILE_FUNCTION_LINE, texture);
			Index(FILE_FUNCTION_LINE, texture, Constants::SAMPLER, Constants::SAMPLERS);
			Integer(FILE_FUNCTION_LINE, texture, Constants::SOURCE, &Validator::GreaterEqualZero);
		}

		void TextureInfo(type_json_object const& textureInfo) {
			Property(FILE_FUNCTION_LINE, textureInfo);
			Integer(FILE_FUNCTION_LINE, textureInfo, Constants::INDEX, &Validator::GreaterEqualZero, true);
			Integer(FILE_FUNCTION_LINE, textureInfo, Constants::TEX_COORD, &Validator::GreaterEqualZero);
		}

		void ExtensionsUsed(type_json_object const& rootObject) {
			ManageBreadCrumb crumbs(*this, Constants::EXTENSIONS_USED);
			type_json_element element = rootObject->Find(Constants::EXTENSIONS_USED);
			std::vector<std::string> used;

			if (element) {
				if (element->type == JsonParse::Type::Array) {
					type_json_array jsonArray = std::static_pointer_cast<JsonParse::JsonArray>(element);
					for (size_t idx = 0; idx < jsonArray->Size(); ++idx) {
						if ((*jsonArray)[idx]->type == JsonParse::Type::String) {
							used.push_back(std::static_pointer_cast<JsonParse::JsonString>((*jsonArray)[idx])->value);
						}
						else {
							ErrorArrayTypeMismatch(FILE_FUNCTION_LINE, idx, JsonParse::Type::String, (*jsonArray)[idx]->type);
						}
					}
				}
				else {
					errors.push_back(GLTFError(rootObject, element, ErrorTypeMismatch(FILE_FUNCTION_LINE, JsonParse::Type::Array, element->type)));
				}
			}

			std::sort(used.begin(), used.end());

			std::vector<std::string>::iterator endOfUnique = std::unique(used.begin(), used.end());
			if (endOfUnique != used.end()) {
				errors.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " must be unique values."));
			}

			// Check all extensions that are used in the file are listed in the extensionsUsed array
			for (std::string const& extension : extensionsInFile) {
				if (std::find(used.cbegin(), used.cend(), extension) == used.cend()) {
					errors.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) +
						" extension:" + extension + " is used in the file but is not in root.extensionsUsed."));
				}
			}

			// Check all extensions listed in the extensiosUsed element are used in the file
			// While this may not be an error it does however make the parsing of the file much simpler if one were to consider it an error
			for (std::vector<std::string>::iterator begin = used.begin(); begin != endOfUnique; ++begin) {
				if (extensionsInFile.find(*begin) == extensionsInFile.cend()) {
					warnings.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " extension:" + *begin + " appears in rootObject.extensionsUsed but is not in the file."));
				}

				if (extensionHandlers.find(*begin) == extensionHandlers.cend()) {
					warnings.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " extension:" + *begin + " is missing a validator."));
				}
			}
		}

		void ExtensionsRequired(type_json_object const& rootObject) {
			ManageBreadCrumb crumbs(*this, Constants::EXTENSIONS_REQUIRED);
			type_json_element element = rootObject->Find(Constants::EXTENSIONS_REQUIRED);
			std::vector<std::string> required;

			if (element) {
				if (element->type == JsonParse::Type::Array) {
					type_json_array jsonArray = std::static_pointer_cast<JsonParse::JsonArray>(element);
					for (size_t idx = 0; idx < jsonArray->Size(); ++idx) {
						if ((*jsonArray)[idx]->type == JsonParse::Type::String) {
							required.push_back(std::static_pointer_cast<JsonParse::JsonString>((*jsonArray)[idx])->value);
						}
						else {
							ErrorArrayTypeMismatch(FILE_FUNCTION_LINE, idx, JsonParse::Type::String, (*jsonArray)[idx]->type);
						}
					}
				}
				else {
					errors.push_back(GLTFError(rootObject, element, ErrorTypeMismatch(FILE_FUNCTION_LINE, JsonParse::Type::Array, element->type)));
				}
			}

			std::sort(required.begin(), required.end());
			std::vector<std::string>::iterator endOfUnique = std::unique(required.begin(), required.end());
			if (endOfUnique != required.end()) {
				errors.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " must be unique values."));
			}

			// Check all extensions listed in the extensionsRequired element are used in the file
			// While this may not be an error it does however make the parsing of the file much simpler if one were to consider it an error
			for (std::vector<std::string>::const_iterator begin = required.cbegin(); begin != endOfUnique; ++begin) {
				if (extensionsInFile.find(*begin) == extensionsInFile.cend()) {
					warnings.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " extension:" + *begin + " appears in rootObject.extensionsRequired but is not in the file."));
				}

				if (extensionHandlers.find(*begin) == extensionHandlers.cend()) {
					warnings.push_back(GLTFError(rootObject, element, ErrorMessageStart(FILE_FUNCTION_LINE) + " extension:" + *begin + " is missing a validator."));
				}
			}
		}

		void Root(type_json_object const& rootObject) {
			Property(FILE_FUNCTION_LINE, rootObject);
			Object(FILE_FUNCTION_LINE, rootObject, Constants::ASSET, &Validator::Asset, true);
			arraySizes[Constants::ACCESSORS] = ArraySize(rootObject, Constants::ACCESSORS);
			arraySizes[Constants::ANIMATIONS] = ArraySize(rootObject, Constants::ANIMATIONS);
			arraySizes[Constants::BUFFERS] = ArraySize(rootObject, Constants::BUFFERS);
			arraySizes[Constants::BUFFER_VIEWS] = ArraySize(rootObject, Constants::BUFFER_VIEWS);
			arraySizes[Constants::CAMERAS] = ArraySize(rootObject, Constants::CAMERAS);
			arraySizes[Constants::IMAGES] = ArraySize(rootObject, Constants::IMAGES);
			arraySizes[Constants::MATERIALS] = ArraySize(rootObject, Constants::MATERIALS);
			arraySizes[Constants::MESHES] = ArraySize(rootObject, Constants::MESHES);
			arraySizes[Constants::NODE] = ArraySize(rootObject, Constants::NODE);
			arraySizes[Constants::SAMPLERS] = ArraySize(rootObject, Constants::SAMPLERS);
			arraySizes[Constants::SCENES] = ArraySize(rootObject, Constants::SCENES);
			arraySizes[Constants::SKINS] = ArraySize(rootObject, Constants::SKINS);
			arraySizes[Constants::TEXTURES] = ArraySize(rootObject, Constants::TEXTURES);

			accessorsInfo.resize(arraySizes[Constants::ACCESSORS]);
			accessorsIterator = accessorsInfo.begin();
			sizeMeshWeights.reserve(arraySizes[Constants::MESHES]);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::ACCESSORS, &Validator::Accessor);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::ANIMATIONS, &Validator::Animation);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::BUFFERS, &Validator::Buffer);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::BUFFER_VIEWS, &Validator::BufferView);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::CAMERAS, &Validator::Camera);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::IMAGES, &Validator::Image);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::MATERIALS, &Validator::Material);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::MESHES, &Validator::Mesh);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::NODE, &Validator::Node);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::SAMPLERS, &Validator::Sampler);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::SCENES, &Validator::Scene);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::SKINS, &Validator::Skin);
			ArrayOfObjects(FILE_FUNCTION_LINE, rootObject, Constants::TEXTURES, &Validator::Texture);

			ExtensionsUsed(rootObject);

			ExtensionsRequired(rootObject);
		}

		operator bool() {
			return errors.empty();
		}
	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~   This Comment Block is to make it easier to catch end of Validator while scrolling   ~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#undef CALLBACK_STRING_ARGS
#undef CALLBACK_NUMBER_ARGS
#undef CALLBACK_INTEGER_ARGS
#undef CALLBACK_ARGS

	/// <summary>
	/// Searches object for the given element.
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected Type of the element</typeparam>
	/// <param name="messagePreamble"></param>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <param name="defaultValue">Default value if the find/convert fails</param>
	/// <returns>Value of the element on success, or default value if the element is missing or of incorrect type</returns>
	template <class _ExpectedTy>
	inline typename _ExpectedTy::value_type Get_Optional_Value(type_json_object const& object, std::string const& elementName, typename _ExpectedTy::value_type defaultValue = typename _ExpectedTy::value_type()) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == _ExpectedTy::Class_Type()) {
			return std::static_pointer_cast<_ExpectedTy>(foundElement)->value;
		}

		// Return default if element is missing or not of correct type
		return defaultValue;
	}

	/// <summary>
	/// Searches object for the given element.
	/// Specialization for JsonNumber, adds conversion for integer to number.
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected Type of the element</typeparam>
	/// <param name="messagePreamble"></param>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <param name="defaultValue">Default value if the find/convert fails</param>
	/// <returns>Value of the element on success, or default value if the element is missing or of incorrect type</returns>
	template <>
	typename JsonParse::JsonNumber::value_type Get_Optional_Value<JsonParse::JsonNumber>(type_json_object const& object, std::string const& elementName, JsonParse::JsonNumber::value_type defaultValue) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement) {
			if (foundElement->type == JsonParse::Type::Number) {
				return std::static_pointer_cast<JsonParse::JsonNumber>(foundElement)->value;
			}
			else {
				if (foundElement->type == JsonParse::Type::Integer) {
					return static_cast<JsonParse::JsonNumber::value_type>(std::static_pointer_cast<JsonParse::JsonInteger>(foundElement)->value);
				}
			}
		}

		// Return default if element is missing or not of correct type
		return defaultValue;
	}

	/// <summary>
	/// Searches object for the given element, throws exception if the element is missing or of the wrong type.
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected type of element</typeparam>
	/// <param name="messagePreamble">FILE_FUNCTION_LINE</param>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <returns>Value of the element.</returns>
	template <class _ExpectedTy>
	inline typename _ExpectedTy::value_type Get_Required_Value(std::string&& messagePreamble, type_json_object const& object, std::string const& elementName) {
		type_json_element foundElement = object->Find(elementName);
		if (!foundElement) {
			throw GltfMissingElement(object, messagePreamble + ": required element \"" + elementName + " is missing.");
		}

		if (foundElement->type != _ExpectedTy::Class_Type()) {
			throw GltfTypeMismatch(object, messagePreamble + ": element \"" + elementName + "\" must be a " + JsonParse_Type_To_String(_ExpectedTy::Class_Type()) + ".");
		}

		return std::static_pointer_cast<_ExpectedTy>(foundElement)->value;
	}

	/// <summary>
	/// Searches object for the given element, throws exception if the element is missing or of the wrong type.
	/// Specialization for JsonNumber, adds conversion for integer to number
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected type of element</typeparam>
	/// <param name="messagePreamble">FILE_FUNCTION_LINE</param>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <returns>Value of the element.</returns>
	template <>
	inline typename JsonParse::JsonNumber::value_type Get_Required_Value<JsonParse::JsonNumber>(std::string&& messagePreamble, type_json_object const& object, std::string const& elementName) {
		type_json_element foundElement = object->Find(elementName);
		if (!foundElement) {
			throw GltfMissingElement(object, messagePreamble + ": required element\"" + elementName + " is missing.");
		}
		if (foundElement->type != JsonParse::Type::Number) {
			if (foundElement->type != JsonParse::Type::Integer) {
				throw GltfTypeMismatch(object, messagePreamble + ": element \"" + elementName + "\" must be a number or integer.");
			}
			return static_cast<JsonParse::JsonNumber::value_type>(std::static_pointer_cast<JsonParse::JsonInteger>(foundElement)->value);
		}

		return std::static_pointer_cast<JsonParse::JsonNumber>(foundElement)->value;
	}

	/// <summary>
	/// Searches object for the given element.
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected type of element</typeparam>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <returns>Converted element, nullptr if conversion fails</returns>
	template <class _ExpectedTy>
	inline std::shared_ptr<_ExpectedTy> Get_Optional_Element(type_json_object const& object, std::string const& elementName) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == _ExpectedTy::Class_Type()) {
			return std::static_pointer_cast<_ExpectedTy>(foundElement);
		}
		else {
			return std::shared_ptr<_ExpectedTy>(nullptr);
		}
	}

	/// <summary>
	/// Searches object for the given element, throws exception if element is missing or of incorrect type.
	/// </summary>
	/// <typeparam name="_ExpectedTy">Expected type of element</typeparam>
	/// <param name="messagePreamble">FILE_FUNCTION_LINE</param>
	/// <param name="object">Object to search</param>
	/// <param name="elementName">Element to find</param>
	/// <returns>Converted element</returns>
	template <class _ExpectedTy>
	inline std::shared_ptr<_ExpectedTy> Get_Required_Element(std::string&& messagePreamble, type_json_object const& object, std::string const& elementName) {
		type_json_element foundElement = object->Find(elementName);
		if (!foundElement) {
			throw GltfMissingElement(object, messagePreamble + ": required element \"" + elementName + "\" not found.");
		}

		if (foundElement->type != _ExpectedTy::Class_Type()) {
			throw GltfTypeMismatch(object, messagePreamble + ": element \"" + elementName + "\" was not of expected type " + JsonParse_Type_To_String(_ExpectedTy::Class_Type()) + ".");
		}

		return std::static_pointer_cast<_ExpectedTy>(foundElement);
	}

	template <class _Ty>
	void Parse_Array_Of_Objects(std::vector<_Ty>& destination, type_json_object const& object, std::string const& elementName, bool required = false) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == JsonParse::Type::Array) {
			type_json_array container = std::static_pointer_cast<JsonParse::JsonArray>(foundElement);

			destination.reserve(container->values.size());
			for (size_t i = 0; i < container->values.size(); ++i) {
				if (container->values[i]->type == JsonParse::Type::Object) {
					destination.emplace_back(std::static_pointer_cast<JsonParse::JsonObject>(container->values[i]));
				}

				// Don't copy on failure
			}
		}
		else {
			if (required) {
				// If foundElement is non-null then the type is wrong
				if (foundElement) {
					throw GltfTypeMismatch(object, FILE_FUNCTION_LINE + ": element \"" + elementName + "\" is not an array.");
				}
				else {
					throw GltfMissingElement(object, FILE_FUNCTION_LINE + ": required element \"" + elementName + "\" is missing.");
				}
			}
		}
	}

	template <class _Ty, class _ExpectedTy>
	void Parse_Array_Dynamic(std::vector<_Ty>& destination, type_json_object const& object, std::string const& elementName, bool required = false) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == JsonParse::Type::Array) {
			type_json_array container = std::static_pointer_cast<JsonParse::JsonArray>(foundElement);

			destination.reserve(container->values.size());
			for (size_t i = 0; i < container->values.size(); ++i) {
				if (container->values[i]->type != _ExpectedTy::Class_Type()) {
					throw GltfTypeMismatch(object, FILE_FUNCTION_LINE + ": element \"" + elementName + "\" idx: " + std::to_string(i) + " element is not of expected type " + JsonParse_Type_To_String(_ExpectedTy::Class_Type()) + ".");
				}

				destination.emplace_back(std::static_pointer_cast<_ExpectedTy>(container->values[i])->value);
			}
		}
		else {
			if (required) {
				if (foundElement) {
					throw GltfTypeMismatch(object, FILE_FUNCTION_LINE + ": element \"" + elementName + "\" is not an array.");
				}
				else {
					throw GltfMissingElement(object, FILE_FUNCTION_LINE + ": required element \"" + elementName + "\" is missing.");
				}
			}
		}
	}

	template <>
	void Parse_Array_Dynamic<number_type, JsonParse::JsonNumber>(std::vector<number_type>& destination, type_json_object const& object, std::string const& elementName, bool required) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == JsonParse::Type::Array) {
			type_json_array container = std::static_pointer_cast<JsonParse::JsonArray>(foundElement);

			destination.reserve(container->values.size());
			for (size_t i = 0; i < container->values.size(); ++i) {
				if (container->values[i]->type != JsonParse::Type::Number) {
					if (container->values[i]->type != JsonParse::Type::Integer) {
						throw GltfTypeMismatch(object, FILE_FUNCTION_LINE + ": element \"" + elementName + "\" idx: " + std::to_string(i) + " element is not of type number or integer.");
					}
					destination.emplace_back(static_cast<number_type>(std::static_pointer_cast<JsonParse::JsonInteger>(container->values[i])->value));
				}
				else {
					destination.emplace_back(std::static_pointer_cast<JsonParse::JsonNumber>(container->values[i])->value);
				}
			}
		}
		else {
			if (required) {
				if (foundElement) {
					throw GltfTypeMismatch(object, FILE_FUNCTION_LINE + ": element \"" + elementName + "\" is not an array.");
				}
				else {
					throw GltfMissingElement(object, FILE_FUNCTION_LINE + ": required element \"" + elementName + "\" is missing.");
				}
			}
		}
	}

	void Parse_Array(std::string&& messagePreamble, number_type* destination, size_t count, type_json_object const& object, std::string const& elementName, bool required = false) {
		type_json_element foundElement = object->Find(elementName);
		if (foundElement && foundElement->type == JsonParse::Type::Array) {
			type_json_array jArray = std::static_pointer_cast<JsonParse::JsonArray>(foundElement);
			if (jArray->values.size() != count) {
				throw GltfArraySizeMismatch(object, messagePreamble + ": element \"" + elementName + "\" array does not contain " + std::to_string(count) + " elements.");
			}
			std::vector<number_type> hold(count, number_type(0));
			int idx = 0;
			for (size_t idx = 0; idx < count; ++idx) {
				if (jArray->values[idx]->type == JsonParse::Type::Number) {
					hold[idx] = std::static_pointer_cast<JsonParse::JsonNumber>(jArray->values[idx])->value;
				}
				else if (jArray->values[idx]->type == JsonParse::Type::Integer) {
					hold[idx] = static_cast<number_type>(std::static_pointer_cast<JsonParse::JsonInteger>(jArray->values[idx])->value);
				}
				else {
					// Don't copy on failure	
					if (required) {
						throw GltfTypeMismatch(object, messagePreamble + ": element \"" + elementName + "\" at index:" + std::to_string(idx) + " is " + JsonParse_Type_To_String(jArray->values[idx]->type) + " must be 'number' or 'integer'.");
					}
					return;
				}
			}
			memcpy_s(destination, count, hold.data(), count);
		}
		else {
			if (required) {
				if (!foundElement) {
					throw GltfMissingElement(object, messagePreamble + ": required element \"" + elementName + "\" is missing.");
				}
				else {
					throw GltfTypeMismatch(object, messagePreamble + ": element \"" + elementName + "\" is not an array.");
				}
			}
		}
	}

	struct __declspec(novtable) GLTFProperty {
		type_json_object extensions;
		type_json_object extras;

		// False if property is default-initialized
		bool definedInFile;

		GLTFProperty(type_json_object const& sourceObject) : definedInFile(true),
			extensions(Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::EXTENSIONS)),
			extras(Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::EXTRAS)) {
		}

		GLTFProperty() : definedInFile(false), extensions(nullptr), extras(nullptr) {

		}

		GLTFProperty(GLTFProperty const&) = default;
		GLTFProperty(GLTFProperty&&) = default;

		GLTFProperty& operator=(GLTFProperty const&) = default;
		GLTFProperty& operator=(GLTFProperty&&) = default;
	};

	struct _declspec(novtable) GLTFRootProperty : GLTFProperty {
		std::string name;

		GLTFRootProperty(type_json_object const& sourceObject) : GLTFProperty(sourceObject), name(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::NAME)) {

		}

		GLTFRootProperty() = default;
		GLTFRootProperty(GLTFRootProperty const&) = default;
		GLTFRootProperty(GLTFRootProperty&&) = default;

		GLTFRootProperty& operator=(GLTFRootProperty const&) = default;
		GLTFRootProperty& operator=(GLTFRootProperty&&) = default;
	};

	struct TextureInfo : public GLTFProperty {
		index_type index = -1;
		index_type texCoord = 0;

		TextureInfo(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
			index(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::INDEX)),
			texCoord(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::TEX_COORD, -1)) {

		}

		TextureInfo() = default;
		TextureInfo(TextureInfo const&) = default;
		TextureInfo(TextureInfo&&) = default;

		TextureInfo& operator=(TextureInfo const&) = default;
		TextureInfo& operator=(TextureInfo&&) = default;
	};

	struct Accessor : public GLTFRootProperty {
		enum Type : unsigned char {
			Error = 0,
			Scalar = 1,
			Vec2,
			Vec3,
			Vec4,
			Mat2x2,
			Mat3x3,
			Mat4x4
		};

		static Type Convert_To_Type(std::string const& typeString) {
			if (typeString == Constants::SCALAR) {
				return Type::Scalar;
			}
			if (typeString == Constants::VEC2) {
				return Type::Vec2;
			}
			if (typeString == Constants::VEC3) {
				return Type::Vec3;
			}
			if (typeString == Constants::VEC4) {
				return Type::Vec4;
			}
			if (typeString == Constants::MAT2) {
				return Type::Mat2x2;
			}
			if (typeString == Constants::MAT3) {
				return Type::Mat3x3;
			}
			if (typeString == Constants::MAT4) {
				return Type::Mat4x4;
			}

			return Type::Error;
		}

		index_type bufferView;
		integer_type byteOffset = 0;
		integer_type componentType;
		bool normalized = false;
		integer_type count;
		Type type;
		std::vector<number_type> max, min;

		struct Sparse : public GLTFProperty {
			integer_type count = -1;
			struct Index : public GLTFProperty {
				index_type bufferView = -1;
				integer_type byteOffset = 0;
				integer_type componentType = Enumerations::ComponentType::Byte;

				Index(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
					bufferView(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::BUFFER_VIEW)),
					byteOffset(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BYTE_OFFSET)),
					componentType(static_cast<decltype(componentType)>(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::COMPONENT_TYPE))) {

				}

				Index() = default;
				Index(Index const&) = default;
				Index(Index&&) = default;

				Index& operator=(Index const&) = default;
				Index& operator=(Index&&) = default;
			};

			struct Value : public GLTFProperty {
				index_type bufferView;
				integer_type byteOffset = 0;

				Value(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
					bufferView(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::BUFFER_VIEW)),
					byteOffset(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BYTE_OFFSET)) {

				}

				Value() = default;
				Value(Value const&) = default;
				Value(Value&&) = default;

				Value& operator=(Value const&) = default;
				Value& operator=(Value&&) = default;
			};

			Index indices;
			Value values;

			Sparse(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
				count(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::COUNT)),
				indices(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::INDICES)),
				values(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::VALUES)) {
			}

			Sparse() = default;
			Sparse(Sparse const&) = default;
			Sparse(Sparse&&) = default;

			Sparse& operator=(Sparse const&) = default;
			Sparse& operator=(Sparse&&) = default;

			size_t IndexSize() const {
				return count * Byte_Per_Component((Enumerations::ComponentType)indices.componentType);
			}
		} sparse;

		Accessor(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			bufferView(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BUFFER_VIEW, -1)),
			byteOffset(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BYTE_OFFSET)),
			componentType(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::COMPONENT_TYPE)),
			normalized(Get_Optional_Value<JsonParse::JsonBoolean>(sourceObject, Constants::NORMALIZED, false)),
			count(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::COUNT)),
			type(Convert_To_Type(Get_Required_Value<JsonParse::JsonString>(FILE_FUNCTION_LINE, sourceObject, Constants::TYPE))) {

			Parse_Array_Dynamic<number_type, JsonParse::JsonNumber>(max, sourceObject, Constants::MAX);
			Parse_Array_Dynamic<number_type, JsonParse::JsonNumber>(min, sourceObject, Constants::MIN);

			type_json_object _sparse = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::SPARSE);
			if (_sparse) {
				sparse = Sparse(_sparse);
			}
		}

		Accessor() = default;
		Accessor(Accessor const&) = default;
		Accessor(Accessor&&) = default;

		Accessor& operator=(Accessor const&) = default;
		Accessor& operator=(Accessor&&) = default;

		static unsigned Component_Count(Type const type) {
			switch (type) {
			case Type::Scalar:
				return 1;
			case Type::Vec2:
				return 2;
			case Type::Vec3:
				return 3;
			case Type::Vec4:
			case Type::Mat2x2:
				return 4;
			case Type::Mat3x3:
				return 9;
			case Type::Mat4x4:
				return 16;
			}
			return 0;
		}

		unsigned ComponentCount() const {
			return Component_Count(this->type);
		}

		static unsigned Byte_Per_Component(Enumerations::ComponentType const componentType) {
			switch (componentType) {
			case Enumerations::ComponentType::Byte:
			case Enumerations::ComponentType::Unsigned_Byte:
				return 1;
			case Enumerations::ComponentType::Short:
			case Enumerations::ComponentType::Unsigned_Short:
				return 2;
			case Enumerations::ComponentType::Int:
			case Enumerations::ComponentType::Unsigned_Int:
			case Enumerations::ComponentType::Float:
				return 4;
			}
			return 0;
		}

		unsigned BytesPerComponent() const {
			return Byte_Per_Component((Enumerations::ComponentType)this->componentType);
		}

		integer_type BytesPerElement() const {
			return ComponentCount() * BytesPerComponent();
		}

		integer_type ByteLength() const {
			return count * BytesPerElement();
		}
	};

	struct Animation : public GLTFRootProperty {
		struct Channel : public GLTFProperty {
			index_type sampler;
			struct Target : public GLTFProperty {
				index_type node;
				std::string path;

				Target(type_json_object sourceObject) : GLTFProperty(sourceObject),
					node(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::NODE, -1)),
					path(Get_Required_Value<JsonParse::JsonString>(FILE_FUNCTION_LINE, sourceObject, Constants::PATH)) {

				}

				Target() = default;
				Target(Target const&) = default;
				Target(Target&&) = default;

				Target& operator=(Target const&) = default;
				Target& operator=(Target&&) = default;
			} target;

			Channel(type_json_object sourceObject) : GLTFProperty(sourceObject),
				sampler(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::SAMPLER)),
				target(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::TARGET)) {

			}

			Channel() = default;
			Channel(Channel const&) = default;
			Channel(Channel&&) = default;

			Channel& operator=(Channel const&) = default;
			Channel& operator=(Channel&&) = default;
		};

		struct Sampler : public GLTFProperty {
			index_type input;
			std::string interpolation = Constants::DEFAULT_INTERPOLATION;
			index_type output;

			Sampler(type_json_object sourceObject) : GLTFProperty(sourceObject),
				input(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::INPUT)),
				interpolation(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::INTERPOLATION, Constants::DEFAULT_INTERPOLATION)),
				output(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::OUTPUT)) {

			}

			Sampler() = default;
			Sampler(Sampler const&) = default;
			Sampler(Sampler&&) = default;

			Sampler& operator=(Sampler const&) = default;
			Sampler& operator=(Sampler&&) = default;
		};

		std::vector<Channel> channels;
		std::vector<Sampler> samplers;

		Animation(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject) {
			Parse_Array_Of_Objects(channels, sourceObject, Constants::CHANNELS);
			Parse_Array_Of_Objects(samplers, sourceObject, Constants::SAMPLERS);
		}

		Animation() = default;
		Animation(Animation const&) = default;
		Animation(Animation&&) = default;

		Animation& operator=(Animation const&) = default;
		Animation& operator=(Animation&&) = default;
	};

	struct Asset : public GLTFProperty {
		std::string copyright;
		std::string generator;
		std::string version;
		std::string minVersion;

		Asset(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
			copyright(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::COPYRIGHT)),
			generator(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::GENERATOR)),
			version(Get_Required_Value<JsonParse::JsonString>(FILE_FUNCTION_LINE, sourceObject, Constants::VERSION)),
			minVersion(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::MIN_VERSION)) {

		}

		Asset() = default;
		Asset(Asset const&) = default;
		Asset(Asset&&) = default;

		Asset& operator=(Asset const&) = default;
		Asset& operator=(Asset&&) = default;
	};

	struct Buffer : public GLTFRootProperty {
		std::string uri;
		integer_type byteLength;

		Buffer(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			uri(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::URI)),
			byteLength(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::BYTE_LENGTH)) {

		}

		Buffer() = default;
		Buffer(Buffer const&) = default;
		Buffer(Buffer&&) = default;

		Buffer& operator=(Buffer const&) = default;
		Buffer& operator=(Buffer&&) = default;
	};

	struct BufferView : public GLTFRootProperty {

		// Required, Index To Buffer
		index_type buffer;
		// Optional, defaults to Zero(0)
		integer_type byteOffset = 0;
		// Required, length from offset to end of view, not end of buffer data
		integer_type byteLength;
		// Optional, defaults to Negative One(-1)
		integer_type byteStride;
		// Optional, defaults to Negative One(-1)
		index_type target;

		BufferView(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			buffer(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::BUFFER)),
			byteOffset(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BYTE_OFFSET)),
			byteLength(Get_Required_Value<JsonParse::JsonInteger>(FILE_FUNCTION_LINE, sourceObject, Constants::BYTE_LENGTH)),
			byteStride(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BYTE_STRIDE, decltype(byteStride)(-1))),
			target(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::TARGET, -1)) {

		}

		BufferView() = default;
		BufferView(BufferView const&) = default;
		BufferView(BufferView&&) = default;

		BufferView& operator=(BufferView const&) = default;
		BufferView& operator=(BufferView&&) = default;
	};

	struct Camera : public GLTFRootProperty {
		bool isPerspective = false;
		struct Orthographic : public GLTFProperty {
			// Required, no limitation
			number_type xmag = 0; 
			// Required, no limitation
			number_type ymag = 0; 
			// Required, must be greater than zero(>0) and greater than znear(>znear)
			number_type zfar = 2; 
			// Required, must be greater than or equal to zero(>=0)
			number_type znear = 1; 

			Orthographic(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
				xmag(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::XMAG)),
				ymag(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::YMAG)),
				zfar(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::ZFAR)),
				znear(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::ZNEAR)) {

			}

			Orthographic() = default;
			Orthographic(Orthographic const&) = default;
			Orthographic(Orthographic&&) = default;

			Orthographic& operator=(Orthographic const&) = default;
			Orthographic& operator=(Orthographic&&) = default;
		} orthographic;

		struct Perspective : public GLTFProperty {
			// Optional, must be greater than zero(0), default negative one(-1)
			number_type aspectRatio = -1;
			// Required, must be greater than zero(0), and should be less than pi radians(180 degress)
			number_type yfov = -1;
			// Required, must be greater than zero(0) and greater than znear(>znear), default infinity(std::numeric_limits::infinity())
			number_type zfar = -1;
			// Required, must be greater than zero and less than zfar(<zfar)
			number_type znear = -1;

			Perspective(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
				aspectRatio(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::ASPECT_RATIO)),
				yfov(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::YFOV)),
				zfar(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::ZFAR, std::numeric_limits<number_type>::infinity())),
				znear(Get_Required_Value<JsonParse::JsonNumber>(FILE_FUNCTION_LINE, sourceObject, Constants::ZNEAR)) {

			}

			Perspective() = default;
			Perspective(Perspective const&) = default;
			Perspective(Perspective&&) = default;

			Perspective& operator=(Perspective const&) = default;
			Perspective& operator=(Perspective&&) = default;
		} perspective;

		std::string type;

		Camera(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			type(Get_Required_Value<JsonParse::JsonString>(FILE_FUNCTION_LINE, sourceObject, Constants::TYPE)) {
			if (type == Constants::PERSPECTIVE) {
				perspective = Perspective(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::PERSPECTIVE));
			}
			else if (type == Constants::ORTHOGRAPHIC){
				orthographic = Orthographic(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::ORTHOGRAPHIC));
			}
			else {
				// Exception, unknown Camera.type
			}
		}

		Camera() = default;
		Camera(Camera const&) = default;
		Camera(Camera&&) = default;

		Camera& operator=(Camera const&) = default;
		Camera& operator=(Camera&&) = default;
	};

	struct Image : public GLTFRootProperty {
		std::string uri;
		std::string mimeType;
		index_type bufferView;

		Image(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			uri(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::URI)),
			mimeType(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::MIME_TYPE)),
			bufferView(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::BUFFER_VIEW, -1)) {

		}

		Image() = default;
		Image(Image const&) = default;
		Image(Image&&) = default;

		Image& operator=(Image const&) = default;
		Image& operator=(Image&&) = default;
	};

	struct Material : public GLTFRootProperty {
		struct PBRMetallicRoughness : public GLTFProperty {
			number_type baseColorFactor[4] = { 1,1,1,1 };
			TextureInfo baseColorTexture;
			number_type metallicFactor = 1;
			number_type roughnessFactor = 1;
			TextureInfo metallicRoughnessTexture;

			PBRMetallicRoughness(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
				metallicFactor(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::METALLIC_FACTOR)),
				roughnessFactor(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::ROUGHNESS_FACTOR)) {
				type_json_array _baseColorFactor = Get_Optional_Element<JsonParse::JsonArray>(sourceObject, Constants::BASE_COLOR_FACTOR);
				type_json_object _baseColorTexture = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::BASE_COLOR_TEXTURE);
				type_json_object _metallicRoughnessTexture = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::METALLIC_ROUGHNESS_TEXTURE);

				Parse_Array(FILE_FUNCTION_LINE, baseColorFactor, 4, sourceObject, Constants::BASE_COLOR_FACTOR);
				if (_baseColorTexture) {
					baseColorTexture = TextureInfo(_baseColorTexture);
				}
				if (_metallicRoughnessTexture) {
					metallicRoughnessTexture = TextureInfo(_metallicRoughnessTexture);
				}
			}

			PBRMetallicRoughness() = default;
			PBRMetallicRoughness(PBRMetallicRoughness const&) = default;
			PBRMetallicRoughness(PBRMetallicRoughness&&) = default;

			PBRMetallicRoughness& operator=(PBRMetallicRoughness const&) = default;
			PBRMetallicRoughness& operator=(PBRMetallicRoughness&&) = default;
		} pbrMetallicRoughness;

		struct NormalTextureInfo : public TextureInfo {
			number_type scale = 1;

			NormalTextureInfo(type_json_object const& sourceObject) : TextureInfo(sourceObject),
				scale(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::SCALE, number_type(1))) {

			}

			NormalTextureInfo() = default;
			NormalTextureInfo(NormalTextureInfo const&) = default;
			NormalTextureInfo(NormalTextureInfo&&) = default;

			NormalTextureInfo& operator=(NormalTextureInfo const&) = default;
			NormalTextureInfo& operator=(NormalTextureInfo&&) = default;
		} normalTexture;

		struct OcclusionTextureInfo : public TextureInfo {
			number_type strength = 1;

			OcclusionTextureInfo(type_json_object const& sourceObject) : TextureInfo(sourceObject),
				strength(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::STRENGTH, number_type(1))) {

			}

			OcclusionTextureInfo() = default;
			OcclusionTextureInfo(OcclusionTextureInfo const&) = default;
			OcclusionTextureInfo(OcclusionTextureInfo&&) = default;

			OcclusionTextureInfo& operator=(OcclusionTextureInfo const&) = default;
			OcclusionTextureInfo& operator=(OcclusionTextureInfo&&) = default;
		} occlusionTexture;

		TextureInfo emissiveTexture;
		number_type emissiveFactor[3] = { 0, 0, 0 };
		std::string alphaMode = Constants::DEFAULT_ALPHA_MODE;
		number_type alphaCutoff = number_type(0.5);

		bool doubleSided = false;

		Material(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			alphaMode(Get_Optional_Value<JsonParse::JsonString>(sourceObject, Constants::ALPHA_MODE, Constants::DEFAULT_ALPHA_MODE)),
			alphaCutoff(Get_Optional_Value<JsonParse::JsonNumber>(sourceObject, Constants::ALPHA_CUTOFF, number_type(0.5))),
			doubleSided(Get_Optional_Value<JsonParse::JsonBoolean>(sourceObject, Constants::DOUBLE_SIDED, false)) {

			type_json_object _pbrMetallicRoughness = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::PBR_METALLIC_ROUGHNESS);
			type_json_object _normalTexture = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::NORMAL_TEXTURE);
			type_json_object _occlusionTexture = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::OCCLUSION_TEXTURE);
			type_json_object _emissiveTexture = Get_Optional_Element<JsonParse::JsonObject>(sourceObject, Constants::EMISSIVE_TEXTURE);

			if (_pbrMetallicRoughness) {
				pbrMetallicRoughness = PBRMetallicRoughness(_pbrMetallicRoughness);
			}
			if (_normalTexture) {
				normalTexture = NormalTextureInfo(_normalTexture);
			}
			if (_occlusionTexture) {
				occlusionTexture = OcclusionTextureInfo(_occlusionTexture);
			}

			if (_emissiveTexture) {
				emissiveTexture = TextureInfo(_emissiveTexture);
			}

			Parse_Array(FILE_FUNCTION_LINE, emissiveFactor, 3, sourceObject, Constants::EMISSIVE_FACTOR);
		}

		Material() = default;
		Material(Material const&) = default;
		Material(Material&&) = default;

		Material& operator=(Material const&) = default;
		Material& operator=(Material&&) = default;
	};

	struct Mesh : public GLTFRootProperty {
		struct Primitive : public GLTFProperty {
			std::unordered_map<std::string, index_type> attributes;
			index_type indices;
			index_type material;
			unsigned short mode = 4;

			std::unordered_map<std::string, std::vector<index_type>> targets;

			Primitive(type_json_object const& sourceObject) : GLTFProperty(sourceObject),
				indices(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::INDICES, -1)),
				material(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::MATERIAL, -1)),
				mode(static_cast<decltype(mode)>(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::MODE, 4))) {

				type_json_object attributesObject = Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, sourceObject, Constants::ATTRIBUTES);
				for (JsonParse::JsonObject::pair_type const& attribute : sourceObject->attributes) {
					if (attribute.second->type != JsonParse::Type::Integer) {
						throw GltfTypeMismatch(sourceObject, FILE_FUNCTION_LINE + ": object \"attributes\" attribute \"" + attribute.first + "\" value is not an integer.");
					}
					else {
						attributes[attribute.first] = std::static_pointer_cast<JsonParse::JsonInteger>(attribute.second)->value;
					}
				}

				type_json_array _targets = Get_Optional_Element<JsonParse::JsonArray>(sourceObject, Constants::TARGETS);
				if (_targets) {
					for (size_t idx = 0; idx < _targets->values.size(); ++idx) {
						if (_targets->values[idx]->type != JsonParse::Type::Object) {
							throw GltfTypeMismatch(sourceObject, FILE_FUNCTION_LINE + ": array \"targets\" value at index:" + std::to_string(idx) + " is not an object.");
						}

						type_json_object target = std::static_pointer_cast<JsonParse::JsonObject>(_targets->values[idx]);

						for (JsonParse::JsonObject::pair_type const& attribute : target->attributes) {
							if (attribute.second->type != JsonParse::Type::Integer) {
								throw GltfTypeMismatch(target, FILE_FUNCTION_LINE + ": element \"targets\" at index: " + std::to_string(idx) + " attribute: \"" + attribute.first + "\" is not an integer.");
							}
							else {
								targets[attribute.first].emplace_back(static_cast<size_t>(std::static_pointer_cast<JsonParse::JsonInteger>(attribute.second)->value));
							}
						}
					}

					if (targets.size() > attributes.size()) {
						// Exception
					}
					for (decltype(targets)::const_reference target : targets) {
						if (target.second.size() != attributes.size()) {
							// Exception
						}
					}
				}
			}

			Primitive() = default;
			Primitive(Primitive const&) = default;
			Primitive(Primitive&&) noexcept = default;

			Primitive& operator=(Primitive const&) = default;
			Primitive& operator=(Primitive&&) = default;
		};

		std::vector<Primitive> primitives;
		std::vector<number_type> weights;

		Mesh(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject) {
			type_json_array _primitives = Get_Required_Element<JsonParse::JsonArray>(FILE_FUNCTION_LINE, sourceObject, Constants::PRIMITIVES);
			for (std::shared_ptr<JsonParse::JsonElement> element : _primitives->values) {
				if (element->type != JsonParse::Type::Object) {
					throw GltfTypeMismatch(sourceObject, FILE_FUNCTION_LINE + ": array \"primitives\" contains an element that is not an object.");
				}
				primitives.emplace_back(std::static_pointer_cast<JsonParse::JsonObject>(element));
			}

			type_json_array _weights = Get_Optional_Element<JsonParse::JsonArray>(sourceObject, Constants::WEIGHTS);
			if (_weights) {
				for (std::shared_ptr<JsonParse::JsonElement> const& element : _weights->values) {
					if (element->type != JsonParse::Type::Number) {
						if (element->type != JsonParse::Type::Integer) {
							throw GltfTypeMismatch(sourceObject, FILE_FUNCTION_LINE + ": array \"weights\" contains an element that is not a number or integer.");
						}
						weights.emplace_back(static_cast<number_type>(std::static_pointer_cast<JsonParse::JsonInteger>(element)->value));
					}
					else {
						weights.emplace_back(std::static_pointer_cast<JsonParse::JsonNumber>(element)->value);
					}
				}
			}
		}

		Mesh() = default;
		Mesh(Mesh const&) = default;
		Mesh(Mesh&&) = default;

		Mesh& operator=(Mesh const&) = default;
		Mesh& operator=(Mesh&&) = default;
	};

	struct Node : public GLTFRootProperty {
		index_type camera;
		std::vector<index_type> children;
		index_type skin;
		number_type matrix[16] = { 
			1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			0, 0, 0, 1 
		};
		index_type mesh;
		number_type rotation[4] = { 0, 0, 0, 1 };
		number_type scale[3] = { 1, 1, 1 };
		number_type translation[3] = { 0, 0, 0 };
		std::vector<number_type> weights;

		Node(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			camera(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::CAMERA, -1)),
			skin(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::SKIN, -1)),
			mesh(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::MESH, -1)) {
			Parse_Array_Dynamic<index_type, JsonParse::JsonInteger>(children, sourceObject, Constants::CHILDREN);
			Parse_Array(FILE_FUNCTION_LINE, matrix, 16, sourceObject, Constants::MATRIX);
			Parse_Array(FILE_FUNCTION_LINE, rotation, 4, sourceObject, Constants::ROTATION);
			Parse_Array(FILE_FUNCTION_LINE, scale, 3, sourceObject, Constants::SCALE);
			Parse_Array(FILE_FUNCTION_LINE, translation, 3, sourceObject, Constants::TRANSLATION);
			Parse_Array_Dynamic<number_type, JsonParse::JsonNumber>(weights, sourceObject, Constants::WEIGHTS);
		}

		Node() = default;
		Node(Node const&) = default;
		Node(Node&&) = default;

		Node& operator=(Node const&) = default;
		Node& operator=(Node&&) = default;
	};

	struct Sampler : public GLTFRootProperty {
		integer_type magFilter, minFilter;
		integer_type wrapS = 10497, wrapT = 10497;
		std::string name;

		Sampler(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			magFilter(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::MAG_FILTER)),
			minFilter(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::MIN_FILTER)),
			wrapS(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::WRAP_S)),
			wrapT(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::WRAP_T)) {

		}

		Sampler() = default;
		Sampler(Sampler const&) = default;
		Sampler(Sampler&&) = default;

		Sampler& operator=(Sampler const&) = default;
		Sampler& operator=(Sampler&&) = default;
	};

	struct Scene : public GLTFRootProperty {
		std::vector<index_type> nodes;

		Scene(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject) {
			Parse_Array_Dynamic<index_type, JsonParse::JsonInteger>(nodes, sourceObject, Constants::NODES);
		}

		Scene() = default;
		Scene(Scene const&) = default;
		Scene(Scene&&) = default;

		Scene& operator=(Scene const&) = default;
		Scene& operator=(Scene&&) = default;
	};

	struct Skin : public GLTFRootProperty {
		index_type inverseBindMatrices;
		index_type skeleton;
		std::vector<index_type> joints;

		Skin(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			inverseBindMatrices(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::INVERSE_BIND_MATRICES, -1)),
			skeleton(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::SKELETON)) {
			Parse_Array_Dynamic<index_type, JsonParse::JsonInteger>(joints, sourceObject, Constants::JOINTS, true);
		}

		Skin() = default;
		Skin(Skin const&) = default;
		Skin(Skin&&) = default;

		Skin& operator=(Skin const&) = default;
		Skin& operator=(Skin&&) = default;
	};

	struct Texture : public GLTFRootProperty {
		index_type sampler;
		index_type source;

		Texture(type_json_object const& sourceObject) : GLTFRootProperty(sourceObject),
			sampler(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::SAMPLER)),
			source(Get_Optional_Value<JsonParse::JsonInteger>(sourceObject, Constants::SOURCE)) {

		}

		Texture() = default;
		Texture(Texture const&) = default;
		Texture(Texture&&) = default;

		Texture& operator=(Texture const&) = default;
		Texture& operator=(Texture&&) = default;
	};

	struct GLTFDoc : public GLTFProperty {
		std::vector<std::string> extensionsUsed;
		std::vector<std::string> extensionsRequired;
		std::vector<Accessor> accessors; // VertexArrayObject::Attribute
		std::vector<Animation> animations; // Based on Skins :D
		Asset asset; // Stuff
		std::vector<Buffer> buffers; // Data
		std::vector<BufferView> bufferViews; // Sections of Data
		std::vector<Camera> cameras; // How to render the scene
		std::vector<Image> images; // Images to be used in rendering
		std::vector<Material> materials; // How to render images
		std::vector<Mesh> meshes; // Vertex data
		std::vector<Node> nodes;
		std::vector<Sampler> samplers;
		index_type scene;
		std::vector<Scene> scenes;
		std::vector<Skin> skins;
		std::vector<Texture> textures;
		std::vector<std::string> errors;

		GLTFDoc(type_json_object const& rootObject) : GLTFProperty(rootObject),
			scene(Get_Optional_Value<JsonParse::JsonInteger>(rootObject, Constants::SCENE, decltype(scene)(-1))) {
			try {
				asset = Asset(Get_Required_Element<JsonParse::JsonObject>(FILE_FUNCTION_LINE, rootObject, Constants::ASSET));
				Parse_Array_Of_Objects(accessors, rootObject, Constants::ACCESSORS);
				Parse_Array_Of_Objects(animations, rootObject, Constants::ANIMATIONS);
				Parse_Array_Of_Objects(buffers, rootObject, Constants::BUFFERS);
				Parse_Array_Of_Objects(bufferViews, rootObject, Constants::BUFFER_VIEWS);
				Parse_Array_Of_Objects(cameras, rootObject, Constants::CAMERAS);
				Parse_Array_Of_Objects(images, rootObject, Constants::IMAGES);
				Parse_Array_Of_Objects(materials, rootObject, Constants::MATERIALS);
				Parse_Array_Of_Objects(meshes, rootObject, Constants::MESHES);
				Parse_Array_Of_Objects(nodes, rootObject, Constants::NODES);
				Parse_Array_Of_Objects(samplers, rootObject, Constants::SAMPLERS);
				Parse_Array_Of_Objects(scenes, rootObject, Constants::SCENES);
				Parse_Array_Of_Objects(skins, rootObject, Constants::SKINS);
				Parse_Array_Of_Objects(textures, rootObject, Constants::TEXTURES);

				type_json_array extUsed = Get_Optional_Element<JsonParse::JsonArray>(rootObject, Constants::EXTENSIONS_USED);
				type_json_array extReq = Get_Optional_Element<JsonParse::JsonArray>(rootObject, Constants::EXTENSIONS_REQUIRED);
				if (extUsed) {
					for (std::shared_ptr<JsonParse::JsonElement> const& element : extUsed->values) {
						if (element->type == JsonParse::Type::String) {
							extensionsUsed.emplace_back(std::static_pointer_cast<JsonParse::JsonString>(element)->value);
						}
					}
				}
				if (extReq) {
					for (std::shared_ptr<JsonParse::JsonElement> const& element : extReq->values) {
						if (element->type == JsonParse::Type::String) {
							extensionsUsed.emplace_back(std::static_pointer_cast<JsonParse::JsonString>(element)->value);
						}
					}
				}
			}
			catch (GltfException const& ex) {
				Validator validator(rootObject);
				errors.emplace_back(ex.what());
				for (decltype(validator.errors)::const_reference error : validator.errors) {
					errors.emplace_back(error.message);
				}
			}
			
			Validate();
		}

		GLTFDoc(GLTFDoc const&) = default;
		GLTFDoc(GLTFDoc&&) = default;

		GLTFDoc& operator=(GLTFDoc const&) = default;
		GLTFDoc& operator=(GLTFDoc&&) = default;

		void Validate() const {

		}
	};
}


#undef CALLBACK_ARGS
#undef CALLBACK_INTEGER_ARGS
#undef CALLBACK_NUMBER_ARGS
#undef CALLBACK_STRING_ARGS
#undef CALLBACK_ARRAY_ARGS
#undef FILE_FUNCTION_LINE