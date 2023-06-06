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

//サンプラーデスクを設定
D3D12_STATIC_SAMPLER_DESC SetSAMPLER_DESC();

/// <summary>
/// ブレンド設定
/// </summary>
/// <param name="BLEND_ALPHA">アルファブレンド</param>
/// <param name="BLEND_SUB">減算合成</param>
/// <param name="BLEND_NEGA">色反転合成</param>
/// <param name="BLEND_NORMAL">半透明合成</param>
void SetBlend(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc,int blend);

class Pipeline
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//object3d初期化
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
	//Sprite初期化
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
	//Sprite初期化
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
	

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//コマンドリストを格納する
	static DirectXCommon* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
};


class ParticlePipeline {
public:
	//エイリアステンプレート
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

	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//コマンドリストを格納する
	static DirectXCommon* directX_;

	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayout;

	ComPtr < ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> gsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr < ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr < ID3DBlob> errorBlob = nullptr; // エラーオブジェクト
};
