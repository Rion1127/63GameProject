#pragma once
#include "math.h"
#include <d3d12.h>
#include <wrl.h>

#include "DirectX.h"
#include "Quaternion.h"
#include "myMath.h"

//�萔�o�b�t�@�p�f�[�^�\���́i�RD�ϊ��s��j
struct ConstBufferDataTransform {
	Matrix4 mat; //3D�ϊ��s��
	Matrix4 viewProj;	//�r���[�ƃv���W�F�N�V���������s��
	Vector3 cameraPos;
};

enum class RotType {
	Euler,
	Quaternion
};

class WorldTransform
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(Vector3 scale) { scale_ = scale; }
	void SetRotation(Vector3 rot) { rotation_ = rot; }
	void SetPosition(Vector3 pos) { position_ = pos; }
	void SetQuaternion(Quaternion q) { quaternion_ = q; }
	void SetRotType(RotType type) { rotType = type; }

	void AddScale(float x, float y, float z);
	void AddRotation(float x, float y, float z);
	void AddPosition(float x, float y, float z);

	void AddScale(Vector3 scale) { scale_ += scale; }
	void AddRotation(Vector3 rot) { rotation_ += rot; }
	void AddPosition(Vector3 pos) { position_ += pos; }
	/// <summary>
	/// ���[���h�g�����X�t�H�[���X�V
	/// </summary>
	/// <param name="viewProjection">�J����</param>
	/// <param name="isBillboard">0 = ����; 1 = �r���{�[�h����; 2 = Y���r���{�[�h</param>
	void Update(uint32_t isBillboard = 0);

	Matrix4 GetMatWorld() { return matWorld_; }
	Vector3 GetWorldPos() { return Vector3(matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2]); }

	//�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform_;
	//�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform* constMapTransform_;
	//�e�I�u�W�F�N�g�ւ̃|�C���^
	WorldTransform* parent_ = nullptr;
	Matrix4* parentRotMat_ = nullptr;
	Matrix4* parentPosMat_ = nullptr;

	//�A�t�B���ϊ����
	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };
	//���[���h�ϊ��s��
	Matrix4 scaleMat_;
	Matrix4 rotMat_;
	Matrix4 posMat_;
	Matrix4 matWorld_;
	RotType rotType;
	//�N�H�[�^�j�I��
	Quaternion quaternion_ = { 0,1,0,0 };
private:
};
