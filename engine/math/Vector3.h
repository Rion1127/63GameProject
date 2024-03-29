#pragma once
#include "Vector2.h"

/**
 * @file Vector3.h
 * @brief Vector3に関することをまとめたクラス
 */

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	//コンストラクタ
	Vector3();								//零ベクトルとする
	Vector3(float x, float y, float z);		//x成分, y成分, z成分を指定しての生成

	//メンバ関数
	float length() const;					//ノルム（長さを求める）
	//float lengthsqrt() const;					//ノルム（長さを求める）
	Vector3 normalize();					//正規化する
	float dot(const Vector3& v)const;		//内積を求める
	Vector3 cross(const Vector3& v)const;	//外積を求める
	Vector3 SetLength(float l);


	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;
	Vector3 operator*(Vector3 v);

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator+=(float v);
	Vector3& operator-=(float v);
	Vector3& operator=(float v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector2& v);
	Vector3& operator-=(const Vector2& v);

	bool operator!=(const Vector3& v);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

Vector3 Lerp(const Vector3 s, const Vector3 e, const float f);
