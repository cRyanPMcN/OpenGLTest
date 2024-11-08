#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Graphics.hpp"
#include "Ply.hpp"
#include "Texture.hpp"

#include <GLAD\gl.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\ext.hpp>
#include <glm\ext\quaternion_transform.hpp>
//#include <stb_image.h>
#include <cstddef>

#include <map>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1
//#include <experimental/filesystem>
#include <filesystem>

#define NOMINMAX
#include <Windows.h>

void FrameBuffer_Size_Callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Error_Callback(int errorCode, const char* message) {
	std::cerr << "GLFW Error: " << message << std::endl;
}

float mixRatio = 0.2f;
glm::quat cameraOrientation(1.0f, 0.0f, 0.0f, 0.0f);
float zoomSensitivity = 0.01f;

glm::vec3 modelPosition(0.0f, 0.0f, -20.0f);

void Key_Event_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_UP && action & (GLFW_PRESS | GLFW_REPEAT)) {
		mixRatio += 0.01f * (mixRatio < 1.0f);
		//aspect += .01f;
		//std::cout << "Aspect=" << aspect << std::endl;
	}

	if (key == GLFW_KEY_DOWN && action & (GLFW_PRESS | GLFW_REPEAT)) {
		mixRatio -= 0.01f * (mixRatio > 0.0f);
		//aspect -= 0.01f;
		//std::cout << "Aspect=" << aspect << std::endl;
	}

	switch (key) {
	case GLFW_KEY_Q:
	case GLFW_KEY_A:
	case GLFW_KEY_W:
	case GLFW_KEY_S:
	case GLFW_KEY_E:
	case GLFW_KEY_D:
	case GLFW_KEY_R:
	case GLFW_KEY_F:
		if (action & (GLFW_PRESS | GLFW_REPEAT)) {
			std::cout << "X=" << cameraOrientation.x << ", Y=" << cameraOrientation.y << ", Z=" << cameraOrientation.z << ", W=" << cameraOrientation.w << std::endl;
			std::cout << "Sensitivity=" << zoomSensitivity << std::endl;
		}
		break;
	};


	if (key == GLFW_KEY_W && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(cos(zoomSensitivity), sin(zoomSensitivity), 0.0f, 0.0f) * cameraOrientation;
	}
	if (key == GLFW_KEY_S && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(cos(-zoomSensitivity), sin(-zoomSensitivity), 0.0f, 0.0f) * cameraOrientation;
	}

	if (key == GLFW_KEY_A && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(cos(zoomSensitivity), 0.0f, sin(zoomSensitivity), 0.0f) * cameraOrientation;
	}

	if (key == GLFW_KEY_D && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(cos(-zoomSensitivity), 0.0f, sin(-zoomSensitivity), 0.0f) * cameraOrientation;
	}

	if (key == GLFW_KEY_Q && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(1.0f, 0.0f, 0.0f, zoomSensitivity) * cameraOrientation;
	}

	if (key == GLFW_KEY_E && action & (GLFW_PRESS | GLFW_REPEAT)) {
		cameraOrientation = glm::quat(-1.0f, 0.0f, 0.0f, zoomSensitivity) * cameraOrientation;
	}

	if (key == GLFW_KEY_R && action & (GLFW_PRESS | GLFW_REPEAT)) {
		zoomSensitivity += 0.01f;
	}

	if (key == GLFW_KEY_F && action & (GLFW_PRESS | GLFW_REPEAT)) {
		zoomSensitivity -= 0.01f;
	}

	if (key == GLFW_KEY_UP && action & (GLFW_PRESS | GLFW_REPEAT)) {
		modelPosition.z += 1.0f;
	}

	if (key == GLFW_KEY_DOWN && action & (GLFW_PRESS | GLFW_REPEAT)) {
		modelPosition.z -= 1.0f;
	}

	if (key == GLFW_KEY_LEFT && action & (GLFW_PRESS | GLFW_REPEAT)) {
		modelPosition.x += 1.0f;
	}

	if (key == GLFW_KEY_RIGHT && action & (GLFW_PRESS | GLFW_REPEAT)) {
		modelPosition.x -= 1.0f;
	}

	cameraOrientation = glm::normalize(cameraOrientation);
}

struct GLError {
	GLenum errorCode;
	bool invalidEnum;
	bool invalidValue;
	bool invalidOperation;
	bool stackOverFlow;
	bool stackUnderFlow;
	bool outOfMemory;

	GLError(GLenum error) {
		errorCode = error;
		invalidEnum = error == GL_INVALID_ENUM;
		invalidValue = error == GL_INVALID_VALUE;
		invalidOperation = error == GL_INVALID_OPERATION;
		stackOverFlow = error == GL_STACK_OVERFLOW;
		stackUnderFlow = error == GL_STACK_UNDERFLOW;
		outOfMemory = error == GL_OUT_OF_MEMORY;
	}

	void operator=(GLenum error){
		errorCode = error;
		invalidEnum = error == GL_INVALID_ENUM;
		invalidValue = error == GL_INVALID_VALUE;
		invalidOperation = error == GL_INVALID_OPERATION;
		stackOverFlow = error == GL_STACK_OVERFLOW;
		stackUnderFlow = error == GL_STACK_UNDERFLOW;
		outOfMemory = error == GL_OUT_OF_MEMORY;
	}

	operator bool() {
		return errorCode == GL_NO_ERROR;
	}
};

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

struct Vertex {
	float x, y, z;
	float nx, ny, nz;
	float s, t;
};

struct DrawArraysIndirectCommand {
	unsigned int  count;
	unsigned int  instanceCount;
	unsigned int  first;
	unsigned int  baseInstance;
};

void Callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << "Something Broke" << std::endl;
}

#include "GLTF.hpp"
#include "GLToolkit.hpp"

struct GLTFObject {
	
};
#include <mutex>

// Change the current path
// Resets path on scope exit
// Dangerous if used in multiple places
struct PathCurrentSetTemporary {
	const std::filesystem::path originalPath;
	//const std::filesystem::path changedPath;
	PathCurrentSetTemporary(std::filesystem::path const& path) : originalPath(std::filesystem::current_path())/*, changedPath(path)*/ {
		std::filesystem::current_path(path);
	}

	~PathCurrentSetTemporary() {
		//if (std::filesystem::current_path() == changedPath) {
		//}
		std::filesystem::current_path(originalPath);
	}
};

void Load_GLB_File(std::vector<GLTFObject>& objectContainer, std::filesystem::path const& path) {
	// Sanity check
	if (path.extension() == ".glb") {
		GLTFObject objectToPush;

		objectContainer.push_back(objectToPush);
	}
}

void AddAttribute(BufferFormat& format, GLTF::Accessor const* accessor, unsigned short count) {
	switch (accessor->componentType) {
	case GLTF::Enumerations::ComponentType::Byte:
		format.AddByte(count, false, accessor->normalized);
		break;
	case GLTF::Enumerations::ComponentType::Unsigned_Byte:
		format.AddUnsignedByte(count, false, accessor->normalized);
		break;
	case GLTF::Enumerations::ComponentType::Short:
		format.AddShort(count, false, accessor->normalized);
		break;
	case GLTF::Enumerations::ComponentType::Unsigned_Short:
		format.AddUnsignedShort(count, false, accessor->normalized);
		break;
	case GLTF::Enumerations::ComponentType::Int:
		format.AddInteger(count, false, accessor->normalized);
		break;
	case GLTF::Enumerations::ComponentType::Unsigned_Int:
		format.AddUnsignedInteger(count, false, accessor->normalized);
		break;
	}
}

template <class _Ty>
void Check_Max_Min_Values(std::vector<unsigned char>& values, GLTF::Accessor const& accessor) {
	if (values.size() / sizeof(_Ty) != accessor.count) {
		// Don't check if it is not safe to check
		return;
	}

	_Ty* valuePointer = reinterpret_cast<_Ty*>(values.data() + accessor.byteOffset);
	_Ty* endPointer = reinterpret_cast<_Ty*>(values.data() + values.size());
	decltype(accessor.ComponentCount()) componentCount(accessor.ComponentCount());
	
	//if (accessor.max.size() == componentCount) {
	//	// Check Max and Min
	//	if (accessor.min.size() == componentCount) {
	//		for (size_t initialIdx = 0; initialIdx < accessor.count; ++initialIdx) {
	//			for (size_t repeater = 0, limit = componentCount; repeater < limit; ++repeater) {
	//				if (valuePointer[initialIdx + repeater] > accessor.max[repeater]) {
	//					valuePointer[initialIdx + repeater] = accessor.max[repeater];
	//				}
	//				if (valuePointer[initialIdx + repeater] < accessor.min[repeater]) {
	//					valuePointer[initialIdx + repeater] = accessor.min[repeater];
	//				}
	//			}
	//		}
	//	}
	//	// Check only max
	//	else {
	//		for (size_t initialIdx = 0; initialIdx < accessor.count; initialIdx += componentCount) {
	//			for (size_t repeater = 0, limit = componentCount; repeater < limit; ++repeater) {
	//				if (valuePointer[initialIdx + repeater] > accessor.max[repeater]) {
	//					valuePointer[initialIdx + repeater] = accessor.max[repeater];
	//				}
	//			}
	//		}
	//	}
	//}
	//else {
	//	// Check only min
	//	if (accessor.min.size() == componentCount) {
	//		for (size_t initialIdx = 0; initialIdx < accessor.count; initialIdx += componentCount) {
	//			for (size_t repeater = 0, limit = componentCount; repeater < limit; ++repeater) {
	//				if (valuePointer[initialIdx + repeater] < accessor.min[repeater]) {
	//					valuePointer[initialIdx + repeater] = accessor.min[repeater];
	//				}
	//			}
	//		}
	//	}
	//	else {
	//		// No check
	//	}
	//}
}

template <class _Ty>
std::vector<_Ty> Subrange_Vector(std::vector<unsigned char>&& source, size_t offset, size_t distance) {
	typename std::vector<_Ty>::const_iterator tempIter = source.cbegin() + offset;
	std::vector<_Ty> copy(tempIter, tempIter + distance);
	copy.resize(distance);
	return copy;
}

GLTFObject Load_GLTF_File(std::filesystem::path const& path) {
	if (std::filesystem::is_directory(path)) {
		// Error?
		// If path.extension returns and empty string then there is no issue
		std::filesystem::directory_iterator iterDir(path);
		if (!iterDir->is_directory() && iterDir->path().extension() == ".gltf" || iterDir->path().extension() == ".glb") {
			Load_GLTF_File(path);
		}
	}
	if (path.extension() == ".gltf") {
		// Load directly;
		JsonParse::JsonReader file(path);
		try {
			std::pair<std::shared_ptr<JsonParse::JsonElement>, JsonParse::JsonReader::Statistics> t = JsonParse::JsonReader::Parse_Json(path);
			// Path to the directory the GLTF file is located in
			std::filesystem::path directoryPath(path.parent_path());
			if (t.first->type == JsonParse::Type::Object) {
				std::shared_ptr<JsonParse::JsonObject> object = std::static_pointer_cast<JsonParse::JsonObject>(t.first);
				GLTF::Validator validate(object);
				if (validate.errors.empty()) {
					GLTF::GLTFDoc doc(object);

					std::vector<std::shared_ptr<GLCamera>> cameras;
					// Cameras can be loaded easily
					for (GLTF::Camera const& camera : doc.cameras) {
						cameras.emplace_back(std::make_shared<GLCamera>(camera));
					}

					std::vector<GLBuffer> buffers;

					struct GLBufferView {
						std::string name;
						size_t targetType;
						GLBuffer const& buffer;
						size_t offsetInBytes;
						size_t lengthInBytes;
						size_t stride;

						GLBufferView(GLTF::BufferView const& bufferView, GLBuffer const& _buffer) : buffer(_buffer), name(bufferView.name),
							targetType(bufferView.target), stride(bufferView.byteStride) {
							offsetInBytes = bufferView.byteOffset;
							lengthInBytes = bufferView.byteLength;
							
						}

						std::pair<decltype(buffer.bufferData)::const_iterator, decltype(buffer.bufferData)::const_iterator> Data() const {
							return std::pair(buffer.bufferData.cbegin() + offsetInBytes, buffer.bufferData.cbegin() + offsetInBytes + lengthInBytes);
						}
					};
					std::vector<GLBufferView> bufferViews;

					for (GLTF::Buffer const& buffer : doc.buffers) {
						buffers.emplace_back(GLBuffer(buffer, directoryPath));
					}

					for (GLTF::BufferView const& bufferView : doc.bufferViews) {
						bufferViews.emplace_back(bufferView, buffers[bufferView.buffer]);
					}

					struct BVAInfo {
						size_t alignment;
						std::vector<GLTF::Accessor const*> accessors;

						BVAInfo() : alignment(0), accessors() {

						}
					};
					std::vector<BVAInfo> accInfo(bufferViews.size());
					
					for (GLTF::Accessor const& accessor : doc.accessors) {
						BVAInfo& info = accInfo[accessor.bufferView];
						if (info.alignment < accessor.BytesPerComponent()) {
							info.alignment = accessor.BytesPerComponent();
						}
						// if accessor.bufferView is undefined
						if (accessor.bufferView == -1) {
							// Create empty bufferData
							buffers.emplace_back(GLBuffer((size_t)(accessor.count) * accessor.BytesPerElement()));
						}
						info.accessors.emplace_back(&accessor);
					}

					std::vector<std::shared_ptr<BufferFormat>> bufferViewFormats;
					
					for (size_t index = 0; index < accInfo.size(); ++index) {
						bufferViewFormats.emplace_back(std::shared_ptr<BufferFormat>(new BufferFormat(accInfo[index].alignment)));
						GLBufferView const& view = bufferViews[index];
						if (view.stride <= 0 && accInfo[index].accessors.size() == 1) {
							bufferViews[index].stride = (*(accInfo[index].accessors.begin()))->BytesPerElement();
						}

						std::vector<unsigned char> bufferData(view.Data().first, view.Data().second);
						std::shared_ptr<BufferFormat> format = bufferViewFormats.back();

						for (GLTF::Accessor const* accessor : accInfo[index].accessors) {
							switch (accessor->ComponentCount()) {
							case 1:
							case 2:
							case 4:
								AddAttribute(*format, accessor, accessor->ComponentCount());
								break;
							case 9:
								AddAttribute(*format, accessor, 3);
								AddAttribute(*format, accessor, 3);
								AddAttribute(*format, accessor, 3);
								break;
							case 16:
								AddAttribute(*format, accessor, 4);
								AddAttribute(*format, accessor, 4);
								AddAttribute(*format, accessor, 4);
								AddAttribute(*format, accessor, 4);
								break;
							}

							if (accessor->sparse.definedInFile) {
								GLTF::Accessor::Sparse const& sparse = accessor->sparse;
								std::vector<unsigned char> values;
								std::vector<unsigned char> indices;
								 
								{
									// Value
									size_t byteOffset = sparse.values.byteOffset;
									size_t distance = byteOffset + sparse.count * accessor->BytesPerElement();
									std::vector<unsigned char>::const_iterator iter = bufferViews[sparse.values.bufferView].Data().first;
									values.assign(iter + byteOffset, iter + distance);
									// Index
									byteOffset = sparse.indices.byteOffset;
									distance = byteOffset + sparse.IndexSize();
									iter = bufferViews[sparse.indices.bufferView].Data().first;
									indices.assign(iter + byteOffset, iter + distance);
								}

								switch (sparse.indices.componentType) {
								case GLTF::Enumerations::ComponentType::Unsigned_Byte:
									for (decltype(sparse.count) idxSparse = 0; idxSparse < sparse.count; ++idxSparse) {
										unsigned char* pointerDestination = bufferData.data() + accessor->byteOffset + (view.stride * indices[idxSparse]);
										memcpy_s(pointerDestination, accessor->BytesPerElement(), values.data() + idxSparse * accessor->BytesPerElement(), accessor->BytesPerElement());
									}
									break;
								case GLTF::Enumerations::ComponentType::Unsigned_Short: {
									unsigned short* idxInBufferView = reinterpret_cast<unsigned short*>(indices.data());
									for (decltype(sparse.count) idxSparse = 0; idxSparse < sparse.count; ++idxSparse) {
										unsigned char* pointerDestination = bufferData.data() + accessor->byteOffset + (view.stride * idxInBufferView[idxSparse]);
										memcpy_s(pointerDestination, accessor->BytesPerElement(), values.data() + idxSparse * accessor->BytesPerElement(), accessor->BytesPerElement());
									}
								}
									break;
								case GLTF::Enumerations::ComponentType::Unsigned_Int: {
									unsigned int* idxInBufferView = reinterpret_cast<unsigned int*>(indices.data());
									for (decltype(sparse.count) idxSparse = 0; idxSparse < sparse.count; ++idxSparse) {
										unsigned char* pointerDestination = bufferData.data() + accessor->byteOffset + (view.stride * idxInBufferView[idxSparse]);
										memcpy_s(pointerDestination, accessor->BytesPerElement(), values.data() + idxSparse * accessor->BytesPerElement(), accessor->BytesPerElement());
									}
									break;
								}
								}
							}

							switch (accessor->componentType) {
							case GLTF::Enumerations::ComponentType::Byte:
								Check_Max_Min_Values<char>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Unsigned_Byte:
								Check_Max_Min_Values<unsigned char>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Short:
								Check_Max_Min_Values<short>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Unsigned_Short:
								Check_Max_Min_Values<unsigned short>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Int:
								Check_Max_Min_Values<int>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Unsigned_Int:
								Check_Max_Min_Values<unsigned int>(bufferData, *accessor);
								break;
							case GLTF::Enumerations::ComponentType::Float:
								Check_Max_Min_Values<float>(bufferData, *accessor);
								break;
							}
						}
					}

					//doc.meshes[0];
					
				}
			}
		}
		catch (...) {

		}
	}

	return GLTFObject();
}

int main(void) {
	/* Initialize the library */
	if (!glfwInit()) {
		std::cerr << "GLFW Init Faliled." << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetErrorCallback(Error_Callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_Callback);
	glfwSetKeyCallback(window, Key_Event_Callback);

	if (!gladLoadGL()) {
		std::cout << "Failed to initialized GLAD." << std::endl;
		return EXIT_FAILURE;
	}

	glDebugMessageCallback(Callback, nullptr);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	std::cout << glGetString(GL_VERSION) << std::endl;

	//float vertices[] = {
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	//};
	
	Shader myShader("./vertex.glsl", "./fragment.glsl");
	
	std::shared_ptr<BufferFormat> format = std::make_shared<BufferFormat>(alignof(Vertex));
	format->AddFloat(3);
	format->AddFloat(3);
	format->AddFloat(2);
	
	std::vector<Vertex> v;
	std::ifstream plySource("binpoa.ply", std::ifstream::binary);
	std::map<unsigned int, std::vector<unsigned int>> indices;
	
	Ply::Parse_Ply_File(plySource, *format, v, indices);
	BufferVertex buffer(v, format);
	VertexArray vertArray(format);
	const VertexArray::container_bindings& bindings = vertArray.GetBindings();
	const VertexArray::Binding& binding = *std::find(bindings.cbegin(), bindings.cend(), format);
	if (!vertArray.SetBuffer(std::make_shared<BufferVertex>(buffer), binding)) {

	}
	
	std::shared_ptr<BufferIndex> ibuff = std::make_shared<BufferIndex>(indices[3]);
	
	vertArray.SetIndexBuffer(ibuff);

	//GLuint VAO;
	//GLuint VBO;
	////GLuint EBO;
	//GLuint bindingIndex = 0;
	//
	//GLError error(glGetError());
	//
	//glCreateVertexArrays(1, &VAO);
	//glCreateBuffers(1, &VBO);
	////glCreateBuffers(1, &EBO);
	//
	//glVertexArrayVertexBuffer(VAO, bindingIndex, VBO, 0, 5 * sizeof(float));
	//glNamedBufferStorage(VBO, sizeof(vertices), &vertices, GL_MAP_READ_BIT);
	//
	////glNamedBufferStorage(EBO, sizeof(indices), &indices, GL_MAP_READ_BIT);
	////glVertexArrayElementBuffer(VAO, EBO);
	//
	//glVertexArrayAttribBinding(VAO, 0, bindingIndex);
	//glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
	//glEnableVertexArrayAttrib(VAO, 0);
	//
	////glVertexArrayAttribBinding(VAO, 1, bindingIndex);
	////glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	////glEnableVertexArrayAttrib(VAO, 0);
	//
	//glVertexArrayAttribBinding(VAO, 2, bindingIndex);
	//glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	//glEnableVertexArrayAttrib(VAO, 2);

	GLuint texture1, texture2;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture1);
	//glTextureParameteri(texture1, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTextureParameteri(texture1, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTextureParameteri(texture1, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTextureParameteri(texture1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glCreateTextures(GL_TEXTURE_2D, 1, &texture2);
	//glTextureParameteri(texture2, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTextureParameteri(texture2, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	//glTextureParameteri(texture2, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTextureParameteri(texture2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint sampler;
	glCreateSamplers(1, &sampler);
	glBindSampler(texture1, sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(texture2, sampler);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./container.jpg", &width, &height, &nrChannels, 0);

	//Texture2D container("./container.jpg");
	//Texture2D awesomeFace("./awesomeface.png");

	glTextureStorage2D(texture1, 2, GL_RGB8, width, height);
	glTextureSubImage2D(texture1, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(texture1);

	stbi_image_free(data);

	data = stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);

	glTextureStorage2D(texture2, 2, GL_RGBA8, width, height);
	glTextureSubImage2D(texture2, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateTextureMipmap(texture2);

	stbi_image_free(data);

	myShader.SetInt("texture1", 0);
	myShader.SetInt("texture2", 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	glfwSwapInterval(0);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearNamedFramebufferfv(0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, );

		myShader.Use();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		glBindTextureUnit(0, texture1);
		glBindTextureUnit(1, texture1);

		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.f);

		glm::vec3 cameraPosition(0.0f, 0.0f, -3.0f);
		//glm::vec4 v(cameraOrientation.x, cameraOrientation.y, cameraOrientation.z, cameraOrientation.w);
		myShader.SetVec3("cameraPosition", cameraPosition);
		myShader.SetVec4("cameraOrientation", cameraOrientation);

		myShader.SetMat4("view", view);
		myShader.SetMat4("projection", projection);
		myShader.SetFloat("ratio", mixRatio);

		vertArray.Bind();
		//glBindVertexArray(VAO);
		glLineWidth(1.5);
		//for (unsigned int i = 0; i < 10; i++) {
			// Quaternion rotate code;
			float angle(float(glfwGetTime()) * glm::radians(20.0f /** i*/) / 2);
			float s = sin(angle);
			float c = cos(angle);
			//glm::vec3 rotateVector(0.0, 1.0f, 0.0f);
			glm::vec3 rotateVector(1.0, 0.3f, 0.5f);
			glm::vec4 modelRotate(1.0f);
			
			//glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, cubePositions[i]);
			//if (i % 3) {
				modelRotate = glm::vec4(rotateVector * s, c);
				//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//}
			//else {
			//	modelRotate = glm::vec4(rotateVector, 0.0f);
			//	//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//}
			modelRotate = glm::normalize(modelRotate);
			//std::cout << "ModelRotate: x=" << modelRotate.x << ", y=" << modelRotate.y << ", z=" << modelRotate.z << ", w=" << modelRotate.w << std::endl;
			//myShader.SetMat4("model", model);
			myShader.SetVec3("modelPosition", modelPosition);
			myShader.SetVec4("modelOrientation", modelRotate);

			//glDrawElements(GL_TRIANGLES, vertArray.bufferIndex->subBuffers[0].subBufferSize, GL_UNSIGNED_INT, nullptr);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration diff = end - start;
		std::chrono::steady_clock::duration base = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(1));
		std::string message("LeanOpenGL, FPS:" + std::to_string(base.count() / diff.count()));
		glfwSetWindowTitle(window, message.c_str());
	}

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	//gladLoaderUnloadGL();
	return EXIT_SUCCESS;
}