#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Collision.h"



bool RayCollision(WorldTransform ray, WorldTransform obj)
{
	//���C�̓����蔻��

	//���[���h���W����
	Vector3 objPos;
	Vector3 rayPos;
	objPos = obj.position;
	rayPos = ray.position;
#pragma region ���C�̓����蔻��
	//���C�̎n�_�ƏI�_����
	Vector3 rayStart;
	Vector3 rayEnd;
	rayStart = {
		ray.position.x,
		ray.position.y,
		ray.position.z - ray.scale.z - 11.0f,
	};
	rayEnd = {
		ray.position.x,
		ray.position.y,
		ray.position.z + ray.scale.z + 11.0f,
	};
	//�n�_�ƏI�_���烌�C�̃x�N�g��(a��)�����߂�
	Vector3 rayVec;
	rayVec = rayEnd - rayStart;
	float raySize;
	raySize = rayVec.length();
	//���K��(a��N)
	rayVec.normalize();
	//���C�ƃI�u�W�F�N�g�̃x�N�g��(b��)�����߂�
	// ���C�̎n�_�ƃI�u�W�F�N�g�ւ̃x�N�g��(b��)�����߂�
	Vector3 ABVec;
	ABVec = {
		objPos.x - rayStart.x,
		objPos.y - rayStart.y,
		objPos.z - rayStart.z
	};

	//b���Ea��N ��ray2ObjLength�Ɋi�[
	float ray2ObjectLength = ABVec.dot(rayVec);

	//Q�����߂� a��N * b���Ea��N + P
	Vector3 Q;
	//Q = rayVec * ABVec.dot(rayVec) + rayPos;
	Q = rayVec * ray2ObjectLength + rayStart;
	//�I�u�W�F�N�g���烌�C�̐���(obj-Q)�����߂�
	Vector3 line;
	line = objPos - Q;
	//�����̒��������߂�
	line.length();

	//�����̒��������a+���a���Z����Γ������Ă�
	bool isHit = false;
	if (line.length() <= obj.scale.x + 4.3f) {
		if (raySize >= ray2ObjectLength) {
			isHit = true;
		}
	}

	if (ray2ObjectLength <= 0) {
		isHit = false;
	}

	return isHit;

#pragma endregion
}
//���Ƌ�
bool BallCollision(const WorldTransform& a, const WorldTransform& b) {
	float x, y, z;
	float r;

	x = (float)pow(b.position.x - a.position.x, 2);
	y = (float)pow(b.position.y - a.position.y, 2);
	z = (float)pow(b.position.z - a.position.z, 2);

	float pos = x + y + z;

	r = (float)pow(a.scale.x + b.scale.x, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//���Ƌ�
bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize) {
	float x, y, z;
	float r;

	x = (float)pow(b.x - a.x, 2);
	y = (float)pow(b.y - a.y, 2);
	z = (float)pow(b.z - a.z, 2);

	float pos = x + y + z;

	r = (float)pow(aSize + bSize, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//���Ƌ�
bool BallCollision(const Sphere& a, const Sphere& b)
{
	float x, y, z;
	float r;

	x = (float)pow(b.center.m128_f32[0] - a.center.m128_f32[0], 2);
	y = (float)pow(b.center.m128_f32[1] - a.center.m128_f32[1], 2);
	z = (float)pow(b.center.m128_f32[2] - a.center.m128_f32[2], 2);

	float pos = x + y + z;

	r = (float)pow(a.radius + b.radius, 2);
	if (pos <= r) {
		return true;
	}
	return false;
}
//���ʂƋ�
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Y���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) return false;

	//�^����_���v�Z
	if (inter) {
		//���ʏ�̍ŋߐړ_���A�c���D�]�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
//�_�ƎO�p�`
void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
//�e�Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//�e�̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) return false;
	//�^����_���v�Z
	if (inter) {
		//�O�p�`��̍ŋߐݒ�p���^����_�Ƃ���
		*inter = p;
	}

	return true;
}

