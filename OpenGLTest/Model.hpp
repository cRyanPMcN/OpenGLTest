#pragma once
#include "Core.hpp"
#include <vector>
#include <immintrin.h>

float DotProduct(Vector3D& left, Vector3D& right) {
	return left.x * right.x + left.y * right.y + left.z * left.z;
}

float Magintude(Vector3D& vector) {
	return sqrt(DotProduct(vector, vector));
}

struct Vertex {
	Vector3D position;
	Vector3D normal;
	//Vector3D colour;
	Vector2D texture;
	Vector3D tangent;
	Vector3D biTangent;
};

struct Texture {
	unsigned long long id;
	char* type;
	char* name;
};

struct Mesh {
	std::vector<Mesh*> subMeshes;
	Mesh* superMesh;
	std::vector<Vertex> vertexes;
	std::vector<std::vector<Vertex>::size_type> indexes;
};