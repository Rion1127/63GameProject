#pragma once
#include "Vector3.h"
#include "DirectX.h"
#include <d3d12.h>
class DirectionalLight
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectionalLight* Create();

	static void StaticInit();

	void Init();

	void Update();

	void Draw(UINT rootParameterIndex);

	void TransferConstBuffer();

	void SetLightDir(const Vector3& lightdir);

	void SetLightColor(const Vector3& color);
	//�L���t���O
	void SetActive(bool active) { this->active = active; }
	//�L���t���O�`�F�b�N
	bool IsAvtive() { return active; }

	Vector3 GetLightDir() { return lightdir; }

	Vector3 GetLightColor() { return lightcolor; }

	struct ConstBufferData {
		Vector3 lightv;		//���C�g�̕�����\���x�N�g��
		float pad1;
		Vector3 lightColor;	//���C�g�̐F
		//float pad2;
		bool active;
	};
private:

	static DirectXCommon* directX;

	ComPtr<ID3D12Resource> constBuff;
	ConstBufferData* constMap;
	//���C�g���������i�P�ʃx�N�g���j
	Vector3 lightdir = { 1,0,0 };
	//���C�g�̐F
	Vector3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty;

	//�L���t���O
	bool active = false;
};

