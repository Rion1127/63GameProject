#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include "math.h"
#include <d3d12.h>
#include <wrl.h>

#include "DirectX.h"
#include "Quaternion.h"

//�萔�o�b�t�@�p�f�[�^�\���́i�RD�ϊ��s��j
struct ConstBufferDataTransform {
	XMMATRIX mat; //3D�ϊ��s��
	XMMATRIX viewProj;	//�r���[�ƃv���W�F�N�V���������s��
	Vector3 cameraPos;
};

class WorldTransform
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	WorldTransform();

	void SetScale(float x,float y,float z);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void AddScale(float x, float y, float z);
	void AddRotation(float x, float y, float z);
	void AddPosition(float x, float y, float z);
	/// <summary>
	/// ���[���h�g�����X�t�H�[���X�V
	/// </summary>
	/// <param name="viewProjection">�J����</param>
	/// <param name="isBillboard">0 = ����; 1 = �r���{�[�h����; 2 = Y���r���{�[�h</param>
	void Update(int isBillboard = 0);

	//�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform* constMapTransform;
	//�e�I�u�W�F�N�g�ւ̃|�C���^
	WorldTransform* parent = nullptr;
	
	//�A�t�B���ϊ����
	Vector3 scale = { 1,1,1 };
	Vector3 rotation = { 0,0,0 };
	Vector3 position = { 0,0,0 };
private:
	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	//�N�H�[�^�j�I��
	Quaternion q = { 0,1,0,0 };
};

