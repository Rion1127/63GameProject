#include "Vector2.h"
#include <cmath>    // sqrt

// コンストラクタ
// 零ベクトルに初期化
Vector2::Vector2() : x(0), y(0) {} 

// x,y成分を指定しての生成
Vector2::Vector2(float x, float y) : x(x), y(y) {}

// ベクトルのノルム(長さ)を求める
float Vector2::length() const
{
	return sqrt(x * x + y * y);
}

// ベクトルを正規化
Vector2& Vector2::normalize()
{
	float len = length();
	//Vector2 vec = *this;
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

// 内積を求める
float Vector2::dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

// 外積を求める
float Vector2::cross(const Vector2& v) const
{
	return x * v.y - y * v.x;
}

// 単項演算子オーバーロード
//単項演算子+を使えるようにする
Vector2 Vector2::operator+() const	
{
	return *this;
}
//	〃	　-を使えるようにする
Vector2 Vector2::operator-() const	
{
	return Vector2(-x, -y);
}

// 代入演算子オーバーロード
//+=を使用できるようにする
Vector2& Vector2::operator+=(const Vector2& v)	
{
	x += v.x;
	y += v.y;
	return *this;
}
//-=を使用できるようにする
Vector2& Vector2::operator-=(const Vector2& v)	
{
	x -= v.x;
	y -= v.y;
	return *this;
}
//ベクトルをS倍にする
Vector2& Vector2::operator*=(float s)	
{
	x *= s;
	y *= s;
	return *this;
}
//ベクトルを1/S倍にする
Vector2& Vector2::operator/=(float s)	
{
	x /= s;
	y /= s;
	return *this;
}

bool Vector2::operator==(const Vector2& v)
{
	if (x == v.x && y == v.y) {
		return true;
	}
	else {
		return false;
	}
}

bool Vector2::operator<(const Vector2& v)
{
	if (x < v.x && y < v.y) {
		return true;
	}
	else {
		return false;
	}
}

bool Vector2::operator>(const Vector2& v)
{
	if (x > v.x && y > v.y) {
		return true;
	}
	else {
		return false;
	}
}

bool Vector2::operator<=(const Vector2& v)
{
	if (x <= v.x && y <= v.y) {
		return true;
	}
	else {
		return false;
	}
}

bool Vector2::operator>=(const Vector2& v)
{
	if (x >= v.x && y >= v.y) {
		return true;
	}
	else {
		return false;
	}
}

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
//足し算ができるようにする
const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}
//引き算ができるようにする
const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}
//S倍を計算できるようにする
const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}
//S倍を計算できるようにする（上とはオペランドの位置が逆）
const Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}
//ベクトルの1/S倍を計算できるようにする
const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}