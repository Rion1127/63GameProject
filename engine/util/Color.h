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

	//�P�����Z�q�I�[�o�[���[�h
	Color operator+() const;
	Color operator-() const;
	Color operator*(const Color& c);
	Color operator/(const Color& c);

	//������Z�q�I�[�o�[���[�h
	Color& operator+=(const Color& v);
	Color& operator-=(const Color& v);
	Color& operator*=(float s);
	Color& operator/=(float s);
};

const Color operator*(const Color& c, float s);
const Color operator/(const Color& c, float s);