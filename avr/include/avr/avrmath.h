#pragma once
namespace AVR
{

class vec3
{
public:
	union {
		struct { float x; float y; float z; };
		struct { float r; float g; float b; };
		float cell[3];
	};
};

class vec2
{
public:
	union {
		struct { float x; float y; };
		struct { float u; float v; };
		float cell[2];
	};
};
}
