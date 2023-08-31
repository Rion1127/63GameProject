#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>


class SwordTrail
{
private:
	struct PosBuffer
	{
		Vector3 head; //���̐�[�̈ʒu
		Vector3 tail; //���̖��[�̈ʒu
	};
	struct SwordTrailVertex
	{
		Vector3 pos = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
	};
private:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	//���݃t���[���ł̌��̈ʒu
	PosBuffer tempPos;
	//�]�����邩�ǂ���
	bool isTransfer_;
public:
	SwordTrail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);
};

