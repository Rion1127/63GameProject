#include "Quaternion.h"
#include <cmath>

//単位Quaternionを返す
Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion identity = { 0,0,0,1 };


	return identity;
}
//共役Quaternionを返す
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
//Quaternionのnormを返す
float Quaternion::Norm()
{
	return (float)sqrt(w * w + x * x + y * y + z * z);
}
//正規化したQuaternionを返す
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
//逆Quaternionを返す
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
//Quaternionの積
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
	//thisとq1の内積
	float dot = this->w * q1.w + this->x * q1.x + this->y * q1.y + this->z * q1.z;
	if (dot < 0)
	{
		result = { -this->x,-this->y,-this->z,-this->w };	//もう片方の回転を利用する
		dot = -dot;		//内積も反転
	}
	else
	{
		//なす角を求める
		float theta = acos(dot);
		float sinPh = sin(theta);

		float sinTheta1subT = (float)sin(theta * (1.0 - t));
		float sinThetaMulT = sin(theta * t);

		if (dot < 0.0 && theta > 3.1415f / 2.0)
		{
			//thetaとsinを使って補間係数を求める
			dot = -this->w * q1.w - this->x * q1.x - this->y * q1.y - this->z * q1.z;

			float s1 = sinTheta1subT / sinPh;
			float s2 = sinThetaMulT / sinPh;
			//それぞれの補間係数を利用して保管後の
			result.x = this->x * s1 - q1.x * s2;
			result.y = this->y * s1 - q1.y * s2;
			result.z = this->z * s1 - q1.z * s2;
			result.w = this->w * s1 - q1.w * s2;
		}
		else
		{
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
//任意軸回転を表すQuaternionの生成
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
//ベクトルをQuaternionで回転させた結果のベクトルを求める
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

	//クォータニオン×ベクトル×共役クォータニオン
	return Vector3(result.x, result.y, result.z);
}

Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//uとvを正規化
	Vector3 uNorm = u;
	uNorm.normalize();
	Vector3 vNorm = v;
	vNorm.normalize();

	//uとvの内積
	float dot = uNorm.dot(vNorm);

	//外積をとる
	Vector3 cross = uNorm.cross(vNorm);

	Vector3 axis = cross.normalize();
	//単位ベクトルで内積をとっているのでacosで角度を求める
	float theta = std::acos(dot);
	//axisとthetaで任意軸回転を作って返す
	return MakeAxisAngle(axis, theta);
}

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Quaternion rot)
{
	Matrix4 result;
	result.UnitMatrix();
	// 平行移動、スケーリング、回転行列作成
	Matrix4 transMat;
	Matrix4 scaleMat;
	Matrix4 rotMat;
	transMat.UnitMatrix();
	scaleMat.UnitMatrix();
	rotMat.UnitMatrix();

	transMat = ConvertTranslationMat(pos);	// 平行移動
	scaleMat = ConvertScalingMat(scale);	// スケーリング
	rotMat = ConvertRotationMat(rot);		// 回転

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
