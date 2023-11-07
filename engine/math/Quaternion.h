#pragma once
#include "Vector3.h"
#include "myMath.h"

/**
 * @file Quaternion.h
 * @brief クォータニオンを使いやすいようにまとめたクラス
 */

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

	Matrix4 UpdateMatrix() {
		
		Quaternion q1 = { x,y,z,w };

		q1.Normalize();

		Matrix4 matRot = {
			(powf(q1.w,2.f) + powf(q1.x,2.f) - powf(q1.y,2.f) - powf(q1.z,2.f)),
			2.f * (q1.x * q1.y + q1.w * q1.z),
			2.f * (q1.x * q1.z - q1.w * q1.y),
			0.f,

			2.f * (q1.x * q1.y - q1.w * q1.z),
			(float)(powf(q1.w,2.f) - powf(q1.x,2.f) + powf(q1.y,2.f) - powf(q1.z,2.f)),
			2.f * (q1.y * q1.z + q1.w * q1.x),
			0.f,

			2.f * (q1.x * q1.z + q1.w * q1.y),
			2.f * (q1.y * q1.z - q1.w * q1.x),
			(float)(powf(q1.w,2.f) - powf(q1.x,2.f) - powf(q1.y,2.f) + powf(q1.z,2.f)),
			0.f,

			0,0,0,1
		};


		return matRot;
	};
	//共役Quaternionを返す
	Quaternion Conjugate() const;
	//Quaternionのnormを返す
	float Norm();
	//正規化したQuaternionを返す
	Quaternion Normalize();
	//逆Quaternionを返す
	Quaternion Inverse();
	//Quaternionの積 
	Quaternion Multiply(const Quaternion& rhs)const;
	Quaternion Slerp(const Quaternion& q1, float t);
	// クォータニオンの掛け算を演算子オーバーロードで定義する
	Quaternion operator*(const Quaternion& other) const;
	bool operator==(const Quaternion& other) const;
	bool operator!=(const Quaternion& other) const;
};
//単位Quaternionを返す
Quaternion IdentityQuaternion();
//任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//uからvへの回転を生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);
Quaternion RotationBetweenVectors(Vector3 start, Vector3 dest);
Quaternion VecToDir(Vector3 vec);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Quaternion rot);
Matrix4 ConvertRotationMat(const Quaternion q); // クォータニオンで回転行列を生成する関数
//オイラー角からクォータニオンへ
Quaternion EulerAnglesToQuaternion(const Vector3& Euler);
//クォータニオンからオイラー角へ
Vector3 QuaternionToEulerAngles(const Quaternion q);
