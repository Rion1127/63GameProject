#include "Quaternion.h"
#include <cmath>

//�P��Quaternion��Ԃ�
Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion identity = { 0,0,0,1 };


	return identity;
}
//����Quaternion��Ԃ�
Quaternion Quaternion::Conjugate() const
{
	Quaternion result = {
	this->x * -1,
	this->y * -1,
	this->z * -1,
	w
	};
	return result;
}
//Quaternion��norm��Ԃ�
float Quaternion::Norm()
{
	return (float)sqrt(w * w + x * x + y * y + z * z);
}
//���K������Quaternion��Ԃ�
Quaternion Quaternion::Normalize()
{
	Quaternion result = {
		this->x / Norm(),
		this->y / Norm(),
		this->z / Norm(),
		this->w / Norm(),
	};

	return result;
}
//�tQuaternion��Ԃ�
Quaternion Quaternion::Inverse()
{
	Quaternion result = {
		this->x / (float)pow(Norm(),2),
		this->y / (float)pow(Norm(),2),
		this->z / (float)pow(Norm(),2),
		this->w / (float)pow(Norm(),2),
	};

	return result;
}
//Quaternion�̐�
Quaternion Quaternion::Multiply(const Quaternion& rhs)const
{
	Quaternion result = {
		this->x * rhs.w + this->w * rhs.x - this->z * rhs.y + this->y * rhs.z,
		this->y * rhs.w + this->z * rhs.x + this->w * rhs.y - this->x * rhs.z,
		this->z * rhs.w - this->y * rhs.x + this->x * rhs.y + this->w * rhs.z,
		this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z
	};

	return result;
}
Quaternion Quaternion::Slerp(const Quaternion& q1, float t)
{
	Quaternion result{};
	//this��q1�̓���
	float dot = this->w * q1.w + this->x * q1.x + this->y * q1.y + this->z * q1.z;
	if (dot < 0) {
		result = { -this->x,-this->y,-this->z,-this->w };	//�����Е��̉�]�𗘗p����
		dot = -dot;		//���ς����]
	}
	else {
		//�Ȃ��p�����߂�
		float theta = acos(dot);
		float sinPh = sin(theta);

		float sinTheta1subT = (float)sin(theta * (1.0 - t));
		float sinThetaMulT = sin(theta * t);

		if (dot < 0.0 && theta > 3.1415f / 2.0) {
			//theta��sin���g���ĕ�ԌW�������߂�
			dot = -this->w * q1.w - this->x * q1.x - this->y * q1.y - this->z * q1.z;
			
			float s1 = sinTheta1subT / sinPh;
			float s2 = sinThetaMulT / sinPh;
			//���ꂼ��̕�ԌW���𗘗p���ĕۊǌ��
			result.x = this->x * s1 - q1.x * s2;
			result.y = this->y * s1 - q1.y * s2;
			result.z = this->z * s1 - q1.z * s2;
			result.w = this->w * s1 - q1.w * s2;
		}
		else {
			float s1, s2;
			s1 = sinTheta1subT / sinPh;
			s2 = sinThetaMulT / sinPh;

			result.x = this->x * s1 + q1.x * s2;
			result.y = this->y * s1 + q1.y * s2;
			result.z = this->z * s1 + q1.z * s2;
			result.w = this->w * s1 + q1.w * s2;
		}
	}
	
	return result;
}
//�C�ӎ���]��\��Quaternion�̐���
Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	float Sin = sin(angle / 2.f);

	Vector3 vector = axis;
	vector = vector.normalize();
	Quaternion result = {
		vector.x * Sin,
		vector.y * Sin,
		vector.z * Sin,
		cos(angle / 2.f)
	};

	return result;
}
//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Quaternion conj = quaternion.Conjugate();

	Quaternion q = {
		vector.x,
		vector.y,
		vector.z,
		0
	};

	Quaternion result = quaternion.Multiply(q);

	result = result.Multiply(conj);

	//�N�H�[�^�j�I���~�x�N�g���~�����N�H�[�^�j�I��
	return Vector3(result.x, result.y, result.z);
}

Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//u��v�𐳋K��
	Vector3 uNorm = u;
	uNorm.normalize();
	Vector3 vNorm = v;
	vNorm.normalize();

	//u��v�̓���
	float dot = uNorm.dot(vNorm);

	//�O�ς��Ƃ�
	Vector3 cross = uNorm.cross(vNorm);

	Vector3 axis = cross.normalize();
	//�P�ʃx�N�g���œ��ς��Ƃ��Ă���̂�acos�Ŋp�x�����߂�
	float theta = std::acos(dot);
	//axis��theta�ŔC�ӎ���]������ĕԂ�
	return MakeAxisAngle(axis, theta);
}

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Quaternion rot)
{
	Matrix4 result;
	result.UnitMatrix();
	// ���s�ړ��A�X�P�[�����O�A��]�s��쐬
	Matrix4 transMat;
	Matrix4 scaleMat;
	Matrix4 rotMat;
	transMat.UnitMatrix();
	scaleMat.UnitMatrix();
	rotMat.UnitMatrix();

	transMat = ConvertTranslationMat(pos);	// ���s�ړ�
	scaleMat = ConvertScalingMat(scale);	// �X�P�[�����O
	rotMat = ConvertRotationMat(rot);		// ��]

	result = scaleMat * rotMat * transMat;

	return result;
}

Matrix4 ConvertRotationMat(const Quaternion q)
{
	Matrix4 result;
	result.UnitMatrix();

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;

	result =
	{
		ww + xx - yy - zz, xy + wz          , xz - wy          ,0.0f,
		xy - wz          , ww - xx + yy - zz, yz + wx          ,0.0f,
		xz + wy          , yz - wx          , ww - xx - yy + zz,0.0f,
		0.0f             ,0.0f              ,0.0f              ,1.0f
	};


	return result;
}
