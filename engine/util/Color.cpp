#include "Color.h"

Color::Color() : r(255), g(255), b(255), a(255)
{
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

Color Color::operator+() const
{
	return *this;
}

Color Color::operator-() const
{
	return Color(-r, -g, -b, -a);
}

Color Color::operator*(const Color& c)
{
	Color result(c);
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;

	return *this;
}

Color Color::operator/(const Color& c)
{
	r /= c.r;
	g /= c.g;
	b /= c.b;
	a /= c.a;
	return Color();
}

Color& Color::operator+=(const Color& v)
{
	r += v.r;
	g += v.g;
	b += v.b;
	a += v.a;
	return *this;
}

Color& Color::operator-=(const Color& v)
{
	r -= v.r;
	g -= v.g;
	b -= v.b;
	a -= v.a;
	return *this;
}

Color& Color::operator*=(float s)
{
	r *= s;
	g *= s;
	b *= s;
	a *= s;
	return *this;
}

Color& Color::operator/=(float s)
{
	r /= s;
	g /= s;
	b /= s;
	a /= s;
	return *this;
}


const Color operator*(const Color& c, float s)
{
	Color temp(c);
	return temp *= s;
}

const Color operator/(const Color& c, float s)
{
	Color temp(c);
	return temp /= s;
}
