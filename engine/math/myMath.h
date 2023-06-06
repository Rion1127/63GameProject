#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

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
};
//ラジアンから角度
float ConvertRadianToAngle(float radian);
//角度からラジアン
float ConvertAngleToRadian(float angle);

const Vector3 SplinePosition(const std::vector<Vector3>& point, size_t startIndex, const float t);
// 始点/終点の座標と ベクトルから、曲線の軌道上の座標を返す
Vector3 GetPoint(Vector3 p0, Vector3 p1, Vector3 v0, Vector3 v1, float t);

float UpAndDown(float oneRoundTime, float range);

const Vector3 operator-(const DirectX::XMFLOAT3 v1, const Vector3 v2);
const Vector3 operator-(const Vector3 v1, const DirectX::XMFLOAT3 v2);
const Vector3 operator+(const Vector3 v1, const DirectX::XMFLOAT3 v2);
const Vector3 operator+(const DirectX::XMFLOAT3 v1, const Vector3 v2);
