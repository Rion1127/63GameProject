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
	void SetActive(bool active) { this->active_ = active; }
	//�L���t���O�`�F�b�N
	bool IsAvtive() { return active_; }

	Vector3 GetLightDir() { return lightdir_; }

	Vector3 GetLightColor() { return lightcolor_; }

	struct ConstBufferData {
		Vector3 lightv;		//���C�g�̕�����\���x�N�g��
		float pad1;
		Vector3 lightColor;	//���C�g�̐F
		//float pad2;
		bool active;
	};
private:
	ComPtr<ID3D12Resource> constBuff_;
	ConstBufferData* constMap_;
	//���C�g���������i�P�ʃx�N�g���j
	Vector3 lightdir_ = { 1,0,0 };
	//���C�g�̐F
	Vector3 lightcolor_ = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty_;

	//�L���t���O
	bool active_ = false;
};

