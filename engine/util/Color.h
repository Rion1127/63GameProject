#pragma once
#include "Vector3.h"
class Color
{
public:
	float r;
	float g;
	float b;
	float a;
public:
	Color();
	Color(float r, float g, float b, float a);
	inline void SetColor(Color col) {
		r = col.r;
		g = col.g;
		b = col.b;
		a = col.a;
	};

	
};

