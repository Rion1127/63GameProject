#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "DirectX.h"

#define BLEND_ALPHA		0x0000
#define BLEND_SUB		0x0001
#define BLEND_NEGA		0x0002
#define BLEND_NORMAL	0x0003

//�T���v���[�f�X�N��ݒ�
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// �u�����h�ݒ�
/// </summary>
/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
/// <param name="BLEND_SUB">���Z����</param>
/// <param name="BLEND_NEGA">�F���]����</param>
/// <param name="BLEND_NORMAL">����������</param>
void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc,int blend);

class Pipeline
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//object3d������
	void Object3dIni(int blend);
private:
#pragma region object3D
	void object3DSetInputLayout();

	void object3DSetRasterizer();

	void object3DSetShader();

	void object3DSetRootSignature();

	void object3DSetOther();

#pragma endregion
public:
	//Sprite������
	void SpriteIni(int blend);
private:
#pragma region Sprite
	void SpriteSetInputLayout();

	void SpriteSetRasterizer();

	void SpriteSetShader();

	void SpriteSetRootSignature();

	void SpriteSetOther();


#pragma endregion
public:
	//Sprite������
	void ToonIni(int blend);
private:
#pragma region Sprite
	void ToonSetInputLayout();

	void ToonSetRasterizer();

	void ToonSetShader();

	void ToonSetRootSignature();

	void ToonSetOther();
public:
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState.Get(); }

private:
	

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�R�}���h���X�g���i�[����
	static DirectXCommon* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};


class ParticlePipeline {
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Init(int blend);

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }

	ID3D12PipelineState* gerPipelineState() { return pipelineState.Get(); }

private:
	void SetInputLayout();

	void SetRasterizer();

	void SetShader();

	void SetRootSignature();

	void SetOther();

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�R�}���h���X�g���i�[����
	static DirectXCommon* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> gsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr < ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};
