#pragma once
#include "BufferFormat.hpp"

struct PackedNormal {
	union {
		int data;
		struct {
			short x : 10;
			short y : 10;
			short z : 10;
			short w : 2;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddPackedInteger(true);
	}
};

struct Vector2 {
	Vector2() : x(0), y(0) {

	}

	Vector2(float x, float y) : x(x), y(y) {

	}

	union {
		float data[2];
		struct {
			float x, y;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddFloat(2);
	}
};

struct Vector3 {
	Vector3() : x(0), y(0), z(0) {

	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {

	}

	union {
		float data[3];
		struct {
			float x, y, z;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddFloat(3);
	}
};

struct Quaternion {
	union {
		float data[4];
		struct {
			float x, y, z, w;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddFloat(4);
	}
};

struct Vector2I {
	union {
		int data[2];
		struct {
			int x, y;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddInteger(2, false);
	}
};

struct Colour {
	union {
		char data[4];
		struct {
			char r, g, b, a;
		};
	};

	static void Describe(BufferFormat& format) {
		format.AddByte(4, true, true);
	}
};

using Color = Colour;