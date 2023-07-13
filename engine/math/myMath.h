#pragma once
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <atlcomcli.h>

struct FLOAT2 {
	float x;
	float y;

	FLOAT2();
	FLOAT2(float x, float y);

	//単項演算子オーバーロード
	FLOAT2 operator+() const;
	FLOAT2 operator-() const;
	FLOAT2 operator*(FLOAT2 v);

	//代入演算子オーバーロード
	FLOAT2& operator+=(const FLOAT2& v);
	FLOAT2& operator-=(const FLOAT2& v);
	FLOAT2& operator*=(float s);
	FLOAT2& operator/=(float s);
};

struct FLOAT3 {
	float x;
	float y;
	float z;

	FLOAT3();
	FLOAT3(float x, float y,float z);

	//単項演算子オーバーロード
	FLOAT3 operator+() const;
	FLOAT3 operator-() const;
	FLOAT3 operator*(FLOAT3 v);

	//代入演算子オーバーロード
	FLOAT3& operator+=(const FLOAT3& v);
	FLOAT3& operator-=(const FLOAT3& v);
	FLOAT3& operator*=(float s);
	FLOAT3& operator/=(float s);

	FLOAT3& operator+=(const FLOAT2& v);
	FLOAT3& operator-=(const FLOAT2& v);

	//代入演算子オーバーロード
	FLOAT3& operator+=(const Vector3& v);
	FLOAT3& operator-=(const Vector3& v);
	FLOAT3& operator*=(const Vector3& v);
	FLOAT3& operator/=(const Vector3& v);
	
};

struct Matrix4 {
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	void UnitMatrix();

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);
	Matrix4 operator*(const Matrix4& m2);
	Matrix4 Inverse();			// 逆行列を返す関数
	Matrix4 Transpose();


	Vector3 GetAxisX() { return Vector3(m[0][0], m[0][1], m[0][2]); }
	Vector3 GetAxisY() { return Vector3(m[1][0], m[1][1], m[1][2]); }
	Vector3 GetAxisZ() { return Vector3(m[2][0], m[2][1], m[2][2]); }
	Vector3 GetTranslation() { return Vector3(m[3][0], m[3][1], m[3][2]); }
};

struct Vector4 {
	float x; // x成分
	float y; // y成分
	float z; // z成分
	float w; // w成分

	// コンストラクタ
	Vector4() : x(0), y(0), z(0), w(0){};                                   // 零ベクトルとする
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}; // x成分, y成分, z成分 を指定しての生成
};

Matrix4 ConvertScalingMat(Vector3 scale);		// スケーリング行列を返す関数
Matrix4 ConvertRotationXAxisMat(float angle);	// 回転行列ｘ軸を返す関数
Matrix4 ConvertRotationYAxisMat(float angle);	// 回転行列ｙ軸を返す関数
Matrix4 ConvertRotationZAxisMat(float angle);	// 回転行列ｚ軸を返す関数
Matrix4 ConvertTranslationMat(const Vector3& pos);	// 平行移動を返す関数

template <class T>
inline T Max(T value, T max)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return value;
	else return max;
}
template <class T>
inline T Min(T value, T min)
{
	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return value;
	else return min;
}
template <class T>
inline T Clamp(T value, T min, T max)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return max;

	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return min;

	// どちらにも当てはまらなかったら値をそのまま返す
	return value;
}

//ラジアンから角度
float Angle(float radian);
//角度からラジアン
float Radian(float angle);

const Vector3 SplinePosition(const std::vector<Vector3>& point, uint32_t startIndex, const float t);
// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
Vector3 GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t);

float UpAndDown(float oneRoundTime, float range);

float Vec2Angle(Vector2 vec);

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Vector3 rot);
Vector4 Vec4MulMat4(Vector4 v, Matrix4 m);

Vector3 getEulerAnglesFromVector(const Vector3& vec);