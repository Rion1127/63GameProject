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

//サンプラーデスクを設定
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// ブレンド設定
/// </summary>
/// <param name="BLEND_ALPHA">アルファブレンド</param>
/// <param name="BLEND_SUB">減算合成</param>
/// <param name="BLEND_NEGA">色反転合成</param>
/// <param name="BLEND_NORMAL">半透明合成</param>
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
	DEPTH_WRITE_MASK_ALL = 1
};
enum TEXTURE_ADDRESS_MODE {	//https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_texture_address_mode
	MODE_WRAP = 1,		//繰り返し（タイリング）
	MODE_MIRROR = 2,	//0 ~ 1は通常, 1 ~ 2は反転 , 2 ~ 3 は通常
	MODE_CLAMP = 3,		//0 ~ 1の間でCLAMPされる
	MODE_BORDER = 4,	//HLSL コードで指定された境界線の色に設定されます。
	MODE_MIRROR_ONCE = 5//
};

enum ShaderType {
	VS,
	PS,
	GS
};
class PipelineObject {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature_;
	// パイプランステート
	ComPtr<ID3D12PipelineState> pipelineStateAdd_;
	ComPtr<ID3D12PipelineState> pipelineStateSub_;
	ComPtr<ID3D12PipelineState> pipelineStateNega_;
	ComPtr<ID3D12PipelineState> pipelineStateAlpha_;

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;
	std::vector<D3D12_ROOT_PARAMETER> rootParams_;

	ComPtr<ID3DBlob> vsBlob_ = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob_ = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob_ = nullptr; // エラーオブジェクト
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