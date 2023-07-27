#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>

#include "DirectX.h"

enum BlendNum {
	ADD,
	SUB,
	NEGA,
	ALPHA,
};

//�T���v���[�f�X�N��ݒ�
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// �u�����h�ݒ�
/// </summary>
/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
/// <param name="BLEND_SUB">���Z����</param>
/// <param name="BLEND_NEGA">�F���]����</param>
/// <param name="BLEND_NORMAL">����������</param>
void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc, uint32_t blend);

enum CULL_MODE {
	NONE = 1,
	FRONT = 2,
	BACK = 3
};
enum TOPOLOGY_TYPE
{
	TOPOLOGY_UNDEFINED = 0,
	TOPOLOGY_POINT = 1,
	TOPOLOGY_LINE = 2,
	TOPOLOGY_TRIANGLE = 3,
	TOPOLOGY_PATCH = 4
};
enum WRIGHT_MASK {
	DEPTH_WRITE_MASK_ZERO = 0,
	DEPTH_WRITE_MASK_ALL = 1,
	DEPTH_ENABLE_FALSE = 2
};
enum TEXTURE_ADDRESS_MODE {	//https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_texture_address_mode
	MODE_WRAP = 1,		//�J��Ԃ��i�^�C�����O�j
	MODE_MIRROR = 2,	//0 ~ 1�͒ʏ�, 1 ~ 2�͔��] , 2 ~ 3 �͒ʏ�
	MODE_CLAMP = 3,		//0 ~ 1�̊Ԃ�CLAMP�����
	MODE_BORDER = 4,	//HLSL �R�[�h�Ŏw�肳�ꂽ���E���̐F�ɐݒ肳��܂��B
	MODE_MIRROR_ONCE = 5//
};

enum ShaderType {
	VS,
	PS,
	GS
};
class PipelineObject {
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;
	// �p�C�v�����X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineStateAdd_;
	ComPtr<ID3D12PipelineState> pipelineStateSub_;
	ComPtr<ID3D12PipelineState> pipelineStateNega_;
	ComPtr<ID3D12PipelineState> pipelineStateAlpha_;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;
	std::vector<D3D12_ROOT_PARAMETER> rootParams_;

	ComPtr<ID3DBlob> vsBlob_ = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob_ = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_ = nullptr; // �G���[�I�u�W�F�N�g
public:

public:
	std::string name_;

	void Create(BlendNum blendNum, CULL_MODE cullmode,
		TOPOLOGY_TYPE topologytype, WRIGHT_MASK depthWriteMasc,
		TEXTURE_ADDRESS_MODE uvMode);

	void Setshader(std::string fileName,ShaderType shadertype);

	void AddrootParams(int32_t addNum);
	void AddrootParamsMultiTexture(int32_t addTexnum, int32_t addNum);
	void AddInputLayout(const char* semanticName, DXGI_FORMAT format,uint32_t index = 0);
public:
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

	ID3D12PipelineState* GetPipelineStateAdd() { return pipelineStateAdd_.Get(); }
	ID3D12PipelineState* GetPipelineStateSub() { return pipelineStateSub_.Get(); }
	ID3D12PipelineState* GetPipelineStateNega() { return pipelineStateNega_.Get(); }
	ID3D12PipelineState* GetPipelineStateAlpha() { return pipelineStateAlpha_.Get(); }
};