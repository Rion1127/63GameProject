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
	Quaternion q = q1;
	q = q.Normalize();
	/*if (*this == q)
	{
		return *this;
	}*/
	if (isfinite(this->x) == false ||
		isfinite(this->y) == false || 
		isfinite(this->z) == false || 
		isfinite(this->w) == false)
	{
		return q1;
	}
	Quaternion result{};
	//this��q1�̓���
	float dot = this->w * q.w + this->x * q.x + this->y * q.y + this->z * q.z;
	if (dot < 0)
	{
		result = { -this->x,-this->y,-this->z,-this->w };	//�����Е��̉�]�𗘗p����
		dot = -dot;		//���ς����]
	}
	else
	{
		//�Ȃ��p�����߂�
		float theta = acos(dot);
		float sinPh = sin(theta);

		float sinTheta1subT = (float)sin(theta * (1.0 - t));
		float sinThetaMulT = sin(theta * t);

		if (dot < 0.0 && theta > 3.1415f / 2.0)
		{
			//theta��sin���g���ĕ�ԌW�������߂�
			dot = -this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;

			float s1 = sinTheta1subT / sinPh;
			float s2 = sinThetaMulT / sinPh;
			//���ꂼ��̕�ԌW���𗘗p���ĕۊǌ��
			result.x = this->x * s1 - q.x * s2;
			result.y = this->y * s1 - q.y * s2;
			result.z = this->z * s1 - q.z * s2;
			result.w = this->w * s1 - q.w * s2;
		}
		else
		{
			float s1 = 0.f;
			float s2 = 0.f;
			//�O�Ŋ���Ȃ��悤�ɂ���
			if (sinPh != 0.f) {
				s1 = sinTheta1subT / sinPh;
				s2 = sinThetaMulT / sinPh;

				result.x = this->x * s1 + q.x * s2;
				result.y = this->y * s1 + q.y * s2;
				result.z = this->z * s1 + q.z * s2;
				result.w = this->w * s1 + q.w * s2;
			}
			else {
				result.x = q.x;
				result.y = q.y;
				result.z = q.z;
				result.w = q.w;
			}
		}
	}

	return result;
}
// �N�H�[�^�j�I���̊|���Z�����Z�q�I�[�o�[���[�h�Œ�`����
Quaternion Quaternion::operator*(const Quaternion& other) const {
	float result_x = w * other.x + x * other.w + y * other.z - z * other.y;
	float result_y = w * other.y - x * other.z + y * other.w + z * other.x;
	float result_z = w * other.z + x * other.y - y * other.x + z * other.w;
	float result_w = w * other.w - x * other.x - y * other.y - z * other.z;
	return Quaternion(result_x, result_y, result_z, result_w);
}

bool Quaternion::operator==(const Quaternion& other) const
{
	if (x == other.x &&
		y == other.y &&
		z == other.z &&
		w == other.w)
	{
		return true;
	}

	return false;
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
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//u��v�𐳋K��
	Vector3 uNorm = u;
	uNorm = uNorm.normalize();
	Vector3 vNorm = v;
	vNorm = vNorm.normalize();

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
Quaternion RotationBetweenVectors(Vector3 start, Vector3 dest)
{
	start = start.normalize();
	dest = dest.normalize();

	float cosTheta = start.dot(dest);
	Vector3 rotationAxis;

	//if (cosTheta < -1 + 0.001f) {
	//	// �x�N�g�������Ε����������Ă������ȃP�[�X�F
	//	// �P�ʉ�]���͂Ȃ��̂ŁA�����Ȃ��̂������܂��B
	//	Vector3 up = { 0,0,1 };
	//	Vector3 right = { 1,0,0 };

	//	rotationAxis = up.cross(start);
	//	if (rotationAxis.length() < 0.01) { // ������x�v�Z�I
	//		rotationAxis = right.cross(start);
	//	}
	//	rotationAxis = rotationAxis.normalize();
	//	return MakeAxisAngle(rotationAxis, Radian(180.0f));
	//}

	rotationAxis = start.cross(dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return Quaternion(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}
Quaternion VecToDir(Vector3 vec)
{
	Quaternion rot1 = RotationBetweenVectors(Vector3(0, 0, 1), vec);

	Vector3 up = { 0,1,0 };
	Vector3 right = vec.cross(up);
	up = right.cross(vec);

	Vector3 newUp = Vector3(rot1.x, rot1.y, rot1.z) * Vector3(0, 1, 0);
	Quaternion rot2 = RotationBetweenVectors(newUp, up);
	Quaternion target = rot2 * rot1;

	return target;
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

	Quaternion q_ = q;
	q_ = q_.Normalize();

	float xx = q_.x * q_.x;
	float yy = q_.y * q_.y;
	float zz = q_.z * q_.z;
	float ww = q_.w * q_.w;
	float xy = q_.x * q_.y * 2.0f;
	float xz = q_.x * q_.z * 2.0f;
	float yz = q_.y * q_.z * 2.0f;
	float wx = q_.w * q_.x * 2.0f;
	float wy = q_.w * q_.y * 2.0f;
	float wz = q_.w * q_.z * 2.0f;

	result =
	{
		ww + xx - yy - zz, xy + wz          , xz - wy          ,0.0f,
		xy - wz          , ww - xx + yy - zz, yz + wx          ,0.0f,
		xz + wy          , yz - wx          , ww - xx - yy + zz,0.0f,
		0.0f             ,0.0f              ,0.0f              ,1.0f
	};


	return result;
}

