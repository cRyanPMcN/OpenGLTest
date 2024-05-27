#pragma once

#ifndef DOUBLE_PRECISION_FLOATING_POINT
using floating_point_type = float;
#else
using floating_point_type = double;
#endif

struct Vector2D {
	using value_type = floating_point_type;
	union {
		value_type data[2];
		struct {
			value_type x, y;
		};
	};
};

struct Vector3D {
	using value_type = floating_point_type;
	union {
		value_type data[3];
		union {
			value_type x, r;
		};
		union {
			value_type y, g;
		};
		union {
			value_type z, b;
		};
	};
};

struct Vector4D {
	using value_type = floating_point_type;
	union {
		value_type data[4];
		union {
			value_type x, r;
		};
		union {
			value_type y, g;
		};
		union {
			value_type z, b;
		};
		union {
			value_type w, a;
		};
	};
};