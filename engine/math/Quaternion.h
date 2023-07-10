#pragma once
#include "Vector3.h"
#include "myMath.h"
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

	Matrix4 UpdateMatrix() {
		Quaternion q1 = {
		(float)(x * sin(w / 2)),
		(float)(y * sin(w / 2)),
		(float)(z * sin(w / 2)),
		(float)(cos(w / 2))
		};

		q1.Normalize();

		Matrix4 matRot = {
			(powf(q1.w,2) + powf(q1.x,2) - powf(q1.y,2) - powf(q1.z,2)),
			2.f * (q1.x * q1.y + q1.w * q1.z),
			2.f * (q1.x * q1.z - q1.w * q1.y),
			0.f,

			2.f * (q1.x * q1.y - q1.w * q1.z),
			(float)(pow(q1.w,2) - pow(q1.x,2) + pow(q1.y,2) - pow(q1.z,2)),
			2.f * (q1.y * q1.z + q1.w * q1.x),
			0.f,

			2.f * (q1.x * q1.z + q1.w * q1.y),
			2.f * (q1.y * q1.z - q1.w * q1.x),
			(float)(pow(q1.w,2) - pow(q1.x,2) - pow(q1.y,2) + pow(q1.z,2)),
			0.f,

			0,0,0,1
		};
		

		return matRot;
	};
	//単位Quaternionを返す
	Quaternion IdentityQuaternion();
	//共役Quaternionを返す
	Quaternion Conjugate() const;
	//Quaternionのnormを返す
	float Norm();
	//正規化したQuaternionを返す
	Quaternion Normalize();
	//逆Quaternionを返す
	Quaternion Inverse();
	//Quaternionの積 
	Quaternion Multiply(const Quaternion& rhs)const ;
	
	Quaternion Slerp(const Quaternion& q1, float t);

};
	

//任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//uからvへの回転を生成
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Quaternion rot);
Matrix4 ConvertRotationMat(const Quaternion q); // クォータニオンで回転行列を生成する関数