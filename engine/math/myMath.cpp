#include "Util.h"
#include "myMath.h"
#define PI 3.141592f

#pragma region FLOAT2
FLOAT2::FLOAT2() : x(0), y(0) {}

FLOAT2::FLOAT2(float x, float y)
{
	this->x = x;
	this->y = y;
}

FLOAT2 FLOAT2::operator+() const
{
	return *this;
}

FLOAT2 FLOAT2::operator-() const
{
	return FLOAT2(-x, -y);
}

FLOAT2 FLOAT2::operator*(FLOAT2 v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

FLOAT2& FLOAT2::operator+=(const FLOAT2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

FLOAT2& FLOAT2::operator-=(const FLOAT2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

FLOAT2& FLOAT2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

FLOAT2& FLOAT2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}
#pragma endregion

#pragma region FLOAT3
FLOAT3::FLOAT3() : x(0), y(0), z(0) {}

FLOAT3::FLOAT3(float x, float y, float z) : x(x), y(y), z(z) {}

FLOAT3 FLOAT3::operator+() const
{
	return *this;
}

FLOAT3 FLOAT3::operator-() const
{
	return FLOAT3(-x, -y, -z);
}

FLOAT3 FLOAT3::operator*(FLOAT3 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FLOAT3& FLOAT3::operator+=(const FLOAT3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

FLOAT3& FLOAT3::operator-=(const FLOAT3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

FLOAT3& FLOAT3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

FLOAT3& FLOAT3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

FLOAT3& FLOAT3::operator+=(const FLOAT2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

FLOAT3& FLOAT3::operator-=(const FLOAT2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}
FLOAT3& FLOAT3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
FLOAT3& FLOAT3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
FLOAT3& FLOAT3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}
FLOAT3& FLOAT3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
#pragma endregion

#pragma region Matrix4
Matrix4::Matrix4()
{
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
}

Matrix4::Matrix4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

void Matrix4::UnitMatrix()
{
	for (uint32_t i = 0; i < 4; i++) {
		m[i][i] = 1.0f;
		for (uint32_t j = 0; j < 4; j++) {
			if (i != j) {
				m[i][j] = 0.0f;
			}
		}
	}
}

Matrix4& Matrix4::operator*=(const Matrix4& m2)
{
	*this = *this * m2;
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& m2)
{
	return {
		m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0],
			m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1],
			m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2],
			m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3],

			m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0],
			m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1],
			m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2],
			m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3],

			m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0],
			m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1],
			m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2],
			m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3],

			m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0],
			m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1],
			m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2],
			m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3]
	};
}

Matrix4 Matrix4::Inverse()
{
	float sweepMat[4][8] = {};
	float tmepNum = 0;

	// �|���o���s��̏�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// �����ł�������s��i�� 4 * 4�j
			sweepMat[i][j] = m[i][j];

			// �P�ʍs��i�E 4 * 4�j
			sweepMat[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// �|���o���@
	for (int i = 0; i < 4; i++)
	{
		// �ő听����T������
		float max = fabsf(sweepMat[i][i]);
		int maxIndex = i;
		for (int j = i + 1; j < 4; j++)
		{
			if (max < fabsf(sweepMat[i][i]))
			{
				max = fabsf(sweepMat[i][i]);
				maxIndex = j;
			}
		}
		// �t�s�񋁂߂邩�ǂ���
		if (fabsf(sweepMat[maxIndex][i]) <= 1.e-50)
		{
			// ���߂�Ȃ��ꍇ�͒P�ʍs���Ԃ�
			Matrix4 identity;
			identity.UnitMatrix();
			return identity;
		}

		// �ΏۂƂȂ�s��̑Ίp������1�ɂ���
		tmepNum = 1 / sweepMat[i][i];
		for (int j = 0; j < 8; j++)
		{
			sweepMat[i][j] *= tmepNum;
		}

		// �ΏۂƂȂ�s��̑Ίp�����ȊO��0�ɂ��邽��
		for (int j = 0; j < 4; j++)
		{
			if (i == j) continue;

			tmepNum = -sweepMat[j][i];
			for (int k = 0; k < 8; k++)
			{
				sweepMat[j][k] += sweepMat[i][k] * tmepNum;
			}
		}
	}

	// �t�s���Ԃ�
	Matrix4 inverseMat;
	inverseMat.UnitMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseMat.m[i][j] = sweepMat[i][j + 4];
		}
	}

	return inverseMat;
}

Matrix4 Matrix4::Transpose()
{
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m[j][i];
		}
	}
	return result;
}

Matrix4 ConvertScalingMat(Vector3 scale)
{
	return
	{
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
}
Matrix4 ConvertRotationXAxisMat(float angle)
{
	return
	{
		1,0,0,0,
		0, cosf(angle),sinf(angle),0,
		0,-sinf(angle),cosf(angle),0,
		0,0,0,1
	};
}
Matrix4 ConvertRotationYAxisMat(float angle)
{
	return
	{
		cosf(angle),0,-sinf(angle),0,
		0,1,0,0,
		sinf(angle),0, cosf(angle),0,
		0,0,0,1
	};
}
Matrix4 ConvertRotationZAxisMat(float angle)
{
	return
	{
		 cosf(angle),sinf(angle),0,0,
		-sinf(angle),cosf(angle),0,0,
		0,0,1,0,
		0,0,0,1
	};
}
Matrix4 ConvertTranslationMat(const Vector3& pos)
{
	return
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pos.x,pos.y,pos.z,1
	};
}
#pragma endregion

#pragma region �ϊ�

//���W�A������p�x
float Angle(float radian) {
	float angle = 180 / PI * radian;
	return angle;
}

//�p�x���烉�W�A��
float Radian(float angle) {
	float radian = PI / 180 * angle;
	return radian;
}

#pragma endregion



float UpAndDown(float oneRoundTime, float range, float timer)
{
	return (sinf(PI * 2.f / oneRoundTime * timer) * range);
}

//float UpAndDown(float oneRoundTime, float range)
//{
//	return (float)(sin(PI * 2 / oneRoundTime * GetNowCount()) * range);
//}

float Vec2Angle(Vector2 vec) {
	float angle = 0;
	if (vec.length() == 0) return angle;
	angle = vec.dot({ 0.0f, 1.0f }) / (vec.length() * Vector2(0.0f, 1.0f).length());
	angle = acos(angle);
	angle = Angle(angle);

	// 180�x�܂ł����v�Z�ł��Ȃ��̂�360�x�܂ł̒l�ɏC��
	if (vec.x < 0) angle = 180.0f + (180.0f - angle);

	// �v�Z���ʂ��I�[�o�[�t���[���Ă��Ȃ�������l���X�V
	if (angle < 0)
	{
		return -1;
	}

	return angle;
}

Matrix4 CalculateWorldMat(const Vector3 pos, const Vector3 scale, const Vector3 rot)
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
	scaleMat = ConvertScalingMat(scale);		// �X�P�[�����O
	rotMat *= ConvertRotationZAxisMat(rot.z);	// z����]
	rotMat *= ConvertRotationXAxisMat(rot.x);	// x����]
	rotMat *= ConvertRotationYAxisMat(rot.y);	// y����]

	result = scaleMat * rotMat * transMat;

	return result;
}

Vector4 Vec4MulMat4(Vector4 v, Matrix4 m)
{
	float w = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3];
	Vector4 result = {
		m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w,
		m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w,
		m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w,
		m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3],
	};

	result.x /= w;
	result.y /= w;
	result.z /= w;
	result.w /= w;

	return result;
}

Vector3 getEulerAnglesFromVector(const Vector3& vec) {
	// �x�N�g���̒������v�Z
	double length_xy = (double)std::sqrt(vec.x * vec.x + vec.y * vec.y);
	double length_xyz = (double)std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	Vector3 result;

	result = {
		std::atan2(vec.y, vec.x),
		std::atan2(-vec.z, (float)length_xy) + Radian(90),
		std::atan2(vec.y, vec.x),
	};
	return result;
}