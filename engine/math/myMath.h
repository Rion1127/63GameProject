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

	//�P�����Z�q�I�[�o�[���[�h
	FLOAT2 operator+() const;
	FLOAT2 operator-() const;
	FLOAT2 operator*(FLOAT2 v);

	//������Z�q�I�[�o�[���[�h
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

	//�P�����Z�q�I�[�o�[���[�h
	FLOAT3 operator+() const;
	FLOAT3 operator-() const;
	FLOAT3 operator*(FLOAT3 v);

	//������Z�q�I�[�o�[���[�h
	FLOAT3& operator+=(const FLOAT3& v);
	FLOAT3& operator-=(const FLOAT3& v);
	FLOAT3& operator*=(float s);
	FLOAT3& operator/=(float s);

	FLOAT3& operator+=(const FLOAT2& v);
	FLOAT3& operator-=(const FLOAT2& v);

	//������Z�q�I�[�o�[���[�h
	FLOAT3& operator+=(const Vector3& v);
	FLOAT3& operator-=(const Vector3& v);
	FLOAT3& operator*=(const Vector3& v);
	FLOAT3& operator/=(const Vector3& v);
	
};

struct Matrix4 {
	// �sx��
	float m[4][4];

	// �R���X�g���N�^
	Matrix4();
	// �������w�肵�Ă̐���
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	void UnitMatrix();

	// ������Z�q�I�[�o�[���[�h
	Matrix4& operator*=(const Matrix4& m2);
};
//���W�A������p�x
float ConvertRadianToAngle(float radian);
//�p�x���烉�W�A��
float ConvertAngleToRadian(float angle);

const Vector3 SplinePosition(const std::vector<Vector3>& point, size_t startIndex, const float t);
// �n�_/�I�_�̍��W�� �x�N�g������A�Ȑ��̋O����̍��W��Ԃ�
Vector3 GetPoint(Vector3 p0, Vector3 p1, Vector3 v0, Vector3 v1, float t);

float UpAndDown(float oneRoundTime, float range);

const Vector3 operator-(const DirectX::XMFLOAT3 v1, const Vector3 v2);
const Vector3 operator-(const Vector3 v1, const DirectX::XMFLOAT3 v2);
const Vector3 operator+(const Vector3 v1, const DirectX::XMFLOAT3 v2);
const Vector3 operator+(const DirectX::XMFLOAT3 v1, const Vector3 v2);
