#include "Quaternion.h"
#include <cmath>

/**
 * @file Quaternion.cpp
 * @brief クォータニオンを使いやすいようにまとめたクラス
 */

//単位Quaternionを返す
Quaternion IdentityQuaternion()
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
	Quaternion q = q1;
	q = q.Normalize();
	/*if (*this == q)
	{
		return *this;
	}*/
	Quaternion result{};
	//thisとq1の内積
	float dot = this->w * q.w + this->x * q.x + this->y * q.y + this->z * q.z;
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
			dot = -this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;

			float s1 = sinTheta1subT / sinPh;
			float s2 = sinThetaMulT / sinPh;
			//それぞれの補間係数を利用して保管後の
			result.x = this->x * s1 - q.x * s2;
			result.y = this->y * s1 - q.y * s2;
			result.z = this->z * s1 - q.z * s2;
			result.w = this->w * s1 - q.w * s2;
		}
		else
		{
			float s1 = 0.f;
			float s2 = 0.f;
			//０で割らないようにする
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

	if (isfinite(result.x) == false ||
		isfinite(result.y) == false ||
		isfinite(result.z) == false ||
		isfinite(result.w) == false)
	{
		return q1;
	}

	return result;
}
// クォータニオンの掛け算を演算子オーバーロードで定義する
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

bool Quaternion::operator!=(const Quaternion& other) const
{
	if (x != other.x &&
		y != other.y &&
		z != other.z &&
		w != other.w)
	{
		return true;
	}

	return false;
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
Quaternion DirectionToDirection(const Vector3& u, const Vector3& v)
{
	//uとvを正規化
	Vector3 uNorm = u;
	uNorm = uNorm.normalize();
	Vector3 vNorm = v;
	vNorm = vNorm.normalize();

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
Quaternion RotationBetweenVectors(Vector3 start, Vector3 dest)
{
	start = start.normalize();
	dest = dest.normalize();

	float cosTheta = start.dot(dest);
	Vector3 rotationAxis;

	//if (cosTheta < -1 + 0.001f) {
	//	// ベクトルが反対方向を向いている特殊なケース：
	//	// 単位回転軸はないので、垂直なものを見つけます。
	//	Vector3 up = { 0,0,1 };
	//	Vector3 right = { 1,0,0 };

	//	rotationAxis = up.cross(start);
	//	if (rotationAxis.length() < 0.01) { // もう一度計算！
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

Quaternion EulerAnglesToQuaternion(const Vector3& Euler)
{
	Quaternion result;
	float cosRoll = cosf(Euler.x / 2.0f);
	float sinRoll = sinf(Euler.x / 2.0f);
	float cosPitch = cosf(Euler.y / 2.0f);
	float sinPitch = sinf(Euler.y / 2.0f);
	float cosYaw = cosf(Euler.z / 2.0f);
	float sinYaw = sinf(Euler.z / 2.0f);

	result.x = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	result.y = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	result.z = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	result.w = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	return result;
}

Vector3 QuaternionToEulerAngles(const Quaternion q)
{
	Vector3 euler;
	// クォータニオンからロールを計算します
	euler.x = atan2f(2.0f * (q.w * q.x + q.y * q.z), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
	// クォータニオンからピッチを計算します
	euler.y = asinf(2.0f * (q.w * q.y - q.z * q.x));
	// クォータニオンからヨーを計算します
	euler.z = atan2f(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (q.y * q.y + q.z * q.z));
	return euler;
}
