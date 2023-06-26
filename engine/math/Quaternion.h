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
	//�P��Quaternion��Ԃ�
	Quaternion IdentityQuaternion();
	//����Quaternion��Ԃ�
	Quaternion Conjugate() const;
	//Quaternion��norm��Ԃ�
	float Norm();
	//���K������Quaternion��Ԃ�
	Quaternion Normalize();
	//�tQuaternion��Ԃ�
	Quaternion Inverse();
	//Quaternion�̐� 
	Quaternion Multiply(const Quaternion& rhs)const ;
	
	Quaternion Slerp(const Quaternion& q1, float t);

};
	

//�C�ӎ���]��\��Quaternion�̐���
Quaternion MakeAxisAngle(const Vector3& axis, float angle);
//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//u����v�ւ̉�]�𐶐�
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v);

