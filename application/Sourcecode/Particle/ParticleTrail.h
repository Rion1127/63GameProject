#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include "Timer.h"
#include "WorldTransform.h"
#include "Texture.h"

class ParticleTrail
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
protected:
	struct SwordTrailVertex
	{
		Vector3 pos = { 0, 0, 0 };
		Vector2 uv = { 0, 0 };
	};
public:
	struct PosBuffer
	{
		Vector3 head; //���̐�[�̈ʒu
		Vector3 tail; //���̖��[�̈ʒu
	};
protected:
	std::vector<PosBuffer> posArray_;
	std::vector<SwordTrailVertex> vertex_;
	uint32_t vertSize_;
	//���݃t���[���ł̌��̈ʒu
	PosBuffer tempPos;

	WorldTransform transform_;
	Texture texture_;

	bool isVisible_;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	ComPtr<ID3D12Resource> vertBuff_;
protected:
	//�f�o�b�O
	bool isStop_;
public:
	ParticleTrail(uint32_t vertSize);
	void Update();
	void SetPos(const Vector3& head, const Vector3& tail);
	void SetIsVisible(bool flag) { isVisible_ = flag; }

	void Draw();
	void DrawImgui();
private:
	//�f�[�^�]��
	void TransferBuff();
};

