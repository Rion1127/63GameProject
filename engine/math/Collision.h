#pragma once
#include "Vector3.h"
#include "WorldTransform.h"
//��
struct Sphere {
	//���S���W
	Vector3 center = {0,0,0};
	//���a
	float radius = 1.0f;

	void SetPos(Vector3 pos) {
		center = pos;
	};

	// �t���O
	bool isActive = true;
};
//����
struct Plane {
	//�@���x�N�g��
	Vector3 normal = { 0,1,0};
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};
//�@���t���O�p�`(���v��肪�\��)
struct Triangle {
	//���_���W�R��
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	//�@���x�N�g��
	Vector3 normal;
	//�@�w�̌v�Z
	void ComputeNormal();
};
//���C(������)
struct Ray {
	//�n�_���W
	Vector3 start = { 0,0,0};
	//����
	Vector3 dir = { 1,0,0};
};

bool RayCollision(WorldTransform ray, WorldTransform obj);

bool BallCollision(const WorldTransform& a, const WorldTransform& b);

bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize);

//���Ƌ�
bool BallCollision(const Sphere& a, const Sphere& b);
//���ʂƋ�
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	Vector3* inter = nullptr);
//�_�ƎO�p�`
void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);
//���Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	Vector3* inter = nullptr);
//���C�ƕ���
bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

//���C�Ɩ@���t���O�p�`�̓����蔻��
bool CheckRay2Traiangle(const Ray& ray, const Triangle& triangle,
	float* distance = nullptr, Vector3* inter = nullptr);

//���C�Ƌ��̓����蔻��
bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
	float* distance = nullptr, Vector3* inter = nullptr);