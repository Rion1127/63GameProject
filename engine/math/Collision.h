#pragma once

//��
struct Sphere {
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;

	void SetPos(const Vector3& pos) {
		center.m128_f32[0] = pos.x;
		center.m128_f32[1] = pos.y;
		center.m128_f32[2] = pos.z;
	};

	// �t���O
	bool isActive = false;
};
//����
struct Plane {
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)��̋���
	float distance = 0.0f;
};
//�@���t���O�p�`(���v��肪�\��)
struct Triangle {
	//���_���W�R��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//�@���x�N�g��
	DirectX::XMVECTOR normal;
};

bool RayCollision(const WorldTransform& ray, const WorldTransform& obj);

bool BallCollision(const WorldTransform& a, const WorldTransform& b);

bool BallCollision(const Vector3& a, const float& aSize, const Vector3& b, const float& bSize);

//���Ƌ�
bool BallCollision(const Sphere& a, const Sphere& b);
//���ʂƋ�
bool Sphere2PlaneCol(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter = nullptr);
//�_�ƎO�p�`
void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
//�e�Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
bool Sphere2TriangleCol(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter = nullptr);
