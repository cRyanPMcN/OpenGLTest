#pragma once
#include "Object.hpp"
#include "VertexArray.hpp"
#include <glm\glm.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <intrin.h>

namespace Mesh {
	struct MeshIndex {
		size_t meshId;
		std::string meshName;
	};

	struct Location {
		float scale;
		struct Position {
			float x, y, z;
		} position;
		struct Orientation {
			float x, y, z, w;
		} orientation;
	};

	struct Bone {
		struct Weight {
			long long index;
			float influence;
		};
	};

	struct Mesh : Object {
		std::vector<std::shared_ptr<BufferVertex>> vertexBuffers;
		std::shared_ptr<BufferIndex> indexBuffer;
		decltype(BufferIndex::subBuffers)::iterator indexSubBuffer;
	};

	struct Model : Object {
		std::vector<std::shared_ptr<Mesh>> meshes;
	};

	struct RenderEntity : Object {
		Location offset;
		std::shared_ptr<Model> model;
		std::vector<std::shared_ptr<RenderEntity>> subModels;
	};

	struct Program;

	struct EntityInstance {
		Location whereToDraw;
		std::shared_ptr<RenderEntity> sourceMesh;
		Program& sourceProgram;
	};

	struct Program : Object {
		// Add mutex and locks for thread-safety
		using instance_container = std::map<std::shared_ptr<Mesh>, std::vector<std::shared_ptr<EntityInstance>>>;
		instance_container meshInstances;
	};

	// Mesh factory stores the information about the mesh
	struct MeshFactory {
		std::shared_ptr<BufferVertex> bufferVertex;
		std::shared_ptr<BufferIndex> bufferIndex;

		std::shared_ptr<Mesh> CreateMesh() {
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(*this);
			return mesh;
		}
	};

	struct MeshInstanced {

	};

	// Multiple meshes are composed in a single VBO
	struct MeshComposite {

	};

	//template <class _Ty>
	//struct Mesh : protected BufferInfo<_Ty> {
	//	using BufferInfo<_Ty>::data;
	//	using BufferInfo<_Ty>::description;
	//	std::vector<unsigned int> indexBuffer;
	//};
	//
	//// Mesh is composed of an abitrary number of Meshes
	//template <class _Ty>
	//struct Composite {
	//	std::vector<BufferInfo> buffers;
	//	std::vector<unsigned int> indexBuffer;
	//};
	//
	//struct Instanced {
	//	//std::vector<BufferInfo<_Ty>> buffers;
	//};
}

struct Camera : public Object {
	glm::vec3 position;
	glm::vec4 orientation;
};


struct Catalog {
	std::map<size_t, VertexArray> vertexArrays;
};

class CatalogBlueprint : public Object {
public:
	struct Item : public Object {
		std::vector<std::shared_ptr<BufferFormat>> wrappedFormat;

		Item(std::shared_ptr<BufferFormat> format) {
			wrappedFormat.push_back(format);
		}

		Item(std::vector<std::shared_ptr<BufferFormat>>& formats) : wrappedFormat(formats) {

		}

		bool operator==(const Item& rhs) {
			return (std::equal(wrappedFormat.cbegin(), wrappedFormat.cend(), rhs.wrappedFormat.cbegin(), [](std::shared_ptr<BufferFormat>& lhs, std::shared_ptr<BufferFormat>& rhs) { return (lhs == rhs || *lhs == *rhs); }));
		}

		bool operator==(std::shared_ptr<BufferFormat> format) {
			return (wrappedFormat.size() == 1 && (wrappedFormat.front() == format || *(wrappedFormat.front()) == *format));
		}

		bool operator==(std::vector<std::shared_ptr<BufferFormat>>& formats) {
			return (std::equal(wrappedFormat.cbegin(), wrappedFormat.cend(), formats.cbegin(), [](std::shared_ptr<BufferFormat>& lhs, std::shared_ptr<BufferFormat>& rhs) { return (lhs == rhs || *lhs == *rhs); }));
		}
	};
private:
	std::map<size_t, std::shared_ptr<Item>> formatsById;
	std::map<std::string, std::shared_ptr<Item>> formatsByName;

	void AddItem(std::shared_ptr<Item> catalogItem) {
		formatsById.emplace(catalogItem->id, catalogItem);
		formatsByName.emplace(catalogItem->name, catalogItem);
	}
public:
	bool AddFormat(std::shared_ptr<BufferFormat> format, std::string name) {
		if (format != nullptr && !Contains(format)) {
			std::shared_ptr<Item> item(std::make_shared<Item>(format));
			item->name = name;
			AddItem(item);
			return true;
		}
		
		return false;
	}

	bool AddFormat(std::vector<std::shared_ptr<BufferFormat>>& format, std::string name) {
		if (std::find_if(format.cbegin(), format.cend(), nullptr) == format.cend() && !Contains(format)) {
			std::shared_ptr<Item> item(std::make_shared<Item>(format));
			item->name = name;
			AddItem(item);
			return true;
		}
		return false;
	}

	bool Contains(std::shared_ptr<BufferFormat>& format) {
		return (std::find(formatsById.cbegin(), formatsById.cend(), format) != formatsById.cend());
	}

	bool Contains(std::vector<std::shared_ptr<BufferFormat>>& format) {
		return (std::find(formatsById.cbegin(), formatsById.cend(), format) != formatsById.cend());
	}

	std::shared_ptr<Item> FindByName(std::string name) {
		decltype(formatsByName)::const_iterator find = formatsByName.find(name);
		if (find == formatsByName.cend()) {
			return nullptr;
		}
		return find->second;
	}

	std::shared_ptr<Item> FindById(size_t id) {
		decltype(formatsById)::const_iterator find = formatsById.find(id);
		if (find == formatsById.cend()) {
			return nullptr;
		}
		return find->second;
	}

	Catalog MakeCatalog() {
		Catalog catalog;
		for (std::pair<const size_t, std::shared_ptr<Item>>& format : formatsById) {
			catalog.vertexArrays.emplace(format.first, format.second->wrappedFormat);
		}

		return catalog;
	}
};

class Renderer {
public:
	struct DeviceContext {

	};

	struct RenderContext {

	};
private:
	struct NoCheck_T {

	};
	std::vector<BufferVertex> buffersVertex;
	std::vector<BufferIndex> buffersIndex;
	std::shared_ptr<CatalogBlueprint> blueprintCatalog;

	template <class _DataTy, class _MeshFactoryTy>
	std::shared_ptr<_MeshFactoryTy> CreateMeshFactorySimple(std::vector<_DataTy> data, std::shared_ptr<BufferFormat> format, NoCheck_T) {
		return std::make_shared<_MeshFactoryTy>(args...);
	}
public:
	Renderer(std::shared_ptr<CatalogBlueprint> blueprints) : blueprintCatalog(blueprints) {

	}

	template <class _Ty>
	void CreateMeshFactorySimple(std::vector<_Ty> data, std::shared_ptr<BufferFormat> format) {
		if (!blueprintCatalog->Contains(format)) {
			
		}
	}

	void CreateMeshFactorySimple(std::string formatName) {
		std::shared_ptr<CatalogBlueprint::Item> result = blueprintCatalog->FindByName(formatName);
		if (result != nullptr) {

		}
		//if (result->wrappedFormat.size() != 1) {
		//	return nullptr;
		//}
	}

	RenderContext MakeCurrent() {
		return RenderContext();
	}

	Catalog GetCatalog() {
		return blueprintCatalog->MakeCatalog();
	}
};

struct RenderContext : public Object {
	Renderer& theRenderer;
	Camera theCamera; // There can be only one
	Catalog vertexArrays;

	RenderContext(Renderer& renderSource) : theRenderer(renderSource), vertexArrays() {
		// MakeCurrent();
		vertexArrays = theRenderer.GetCatalog();
	}
};

struct GraphicsEntity : public Object {
	size_t format;
	std::shared_ptr<BufferVertex> vertexBuffer;
	unsigned int vertexBinding;

	glm::vec3 position;
	glm::vec4 orientation;
	float scale;

	GraphicsEntity(unsigned int binding) : vertexBinding(binding) {
		
	}

	void Draw(Catalog& catalog) {
		catalog.vertexArrays[format].SetBuffer(vertexBuffer, vertexBinding);
	}
};