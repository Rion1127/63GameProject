#include "Vector3.h"
#include <cmath>    // sqrt

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
// ベクトルのノルム(長さ)を求める
float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}
//float Vector3::length() const
//{
//	return x * x + y * y + z * z;
//}
// ベクトルを正規化
Vector3 Vector3::normalize()
{
	Vector3 result = *this;
	float len = length();
	if (len != 0)
	{
		result /= len;
		return result;
	}
	return result;
}
// 内積を求める
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}
// 外積を求める
Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}
Vector3 Vector3::SetLength(float l)
{
	float len2 = this->length();
	float mul = l / len2;
	x *= mul;
	y *= mul;
	z *= mul;

	return *this;
}
// 単項演算子オーバーロード
//単項演算子+を使えるようにする
Vector3 Vector3::operator+() const
{
	return *this;
}
//	〃	　-を使えるようにする
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(Vector3 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

// 代入演算子オーバーロード
//+=を使用できるようにする
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
//-=を使用できるようにする
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
//ベクトルをS倍にする
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
//ベクトルを1/S倍にする
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3& Vector3::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector3& Vector3::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

Vector3 Lerp(const Vector3 s, const Vector3 e, const float f)
{
	Vector3 dis = e - s;
	return dis * f + s;
}