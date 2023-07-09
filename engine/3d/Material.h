#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <string>

#include <cassert>
#include "Texture.h"
#include "Vector3.h"

class Material
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Material();

	void LoadTexture(const std::string& directoryPath);
	void SetTexture(const Texture texture) { texture_ = texture; }

	inline void SetAmbient(const Vector3& Ambient) {
		ambient_ = Ambient;
		constMapMat_->ambient = ambient_;
	}
	inline void SetDiffuse(const Vector3& Diffuse) {
		diffuse_ = Diffuse;
		constMapMat_->diffuse = diffuse_;
	}
	inline void SetSpecular(const Vector3& Specular) {
		specular_ = Specular;
		constMapMat_->specular = specular_;
	}

	

	void Draw(UINT descriptorSize);
	void Draw();

	std::string name_;            // �}�e���A����
	std::string textureFilename_; // �e�N�X�`���t�@�C����
	// �e�N�X�`���n���h��
	Texture texture_;
private:

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBuffMaterial {
		Vector3 ambient = { 0.3f,0.3f,0.3f };
		float pad1;
		Vector3 diffuse = { 0.8f,0.8f,0.8f };
		float pad2;
		Vector3 specular = { 0.0f, 0.0f, 0.0f };
		float alpha;
	};
	//�萔�o�b�t�@�̐���
	ComPtr<ID3D12Resource> constBufferMat_ = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBuffMaterial* constMapMat_ = nullptr;

	Vector3 ambient_ = { 0.3f,0.3f,0.3f };
	Vector3 diffuse_ = { 0.8f,0.8f,0.8f };
	Vector3 specular_ = { 0.0f, 0.0f, 0.0f };
	float alpha_ = 1.0f;
};

