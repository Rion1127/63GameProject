#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cassert>
#include <string>
#include "WorldTransform.h"
#include "Material.h"
#include "Util.h"
#include "Particle.h"
#include "DirectX.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "PipelineManager.h"


Particle::Particle() {
	Ini();

	PipelineManager::AddPipeline("Particle");
	PipelineManager::GetPipelineObjects("Particle")->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	PipelineManager::GetPipelineObjects("Particle")->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT);
	PipelineManager::GetPipelineObjects("Particle")->AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);

	PipelineManager::GetPipelineObjects("Particle")->Setshader("ParticleVS.hlsl", ShaderType::VS);
	PipelineManager::GetPipelineObjects("Particle")->Setshader("ParticlePS.hlsl", ShaderType::PS);
	PipelineManager::GetPipelineObjects("Particle")->Setshader("ParticleGS.hlsl", ShaderType::GS);

	PipelineManager::Create("Particle", NONE, TOPOLOGY_POINT, DEPTH_WRITE_MASK_ZERO,MODE_WRAP);
}

Particle* Particle::GetInstance()
{
	static Particle instance;
	return &instance;
}

void Particle::Ini()
{
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * CVERTEX_COUNT);

	////頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_)
	);
	vertBuff_->SetName(L"PARTICLE VERT BUFF");

	// GPU上のバッファに対応した仮想メモリを取得
	vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	//テスト
	vertMap_->pos = { 0,0,0 };

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(Vertex);


	constBuff_ = CreateBuff(constMatMap_);
}

void Particle::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	//パイプラインに設定した内容で描画を始める
	RDirectX::GetInstance()->GetCommandList()->
		SetPipelineState(PipelineManager::GetPipelineObjects("Particle")->GetPipelineStateAlpha());
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(PipelineManager::GetPipelineObjects("Particle")->GetRootSignature());

	// プリミティブ形状の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // ポイントリスト
}

void Particle::Update(Camera VP)
{
	HRESULT result;

	constMatMap_->mat = VP.GetMatView() * VP.GetMatProjection();
	constMatMap_->billBoardMat.UnitMatrix();


	//頂点バッファへデータ送信
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		//パーティクルの情報を一つずつ反映
		for (/*std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++*/uint32_t i = 0; i < activeCount_;i++) {
			//座標
			vertMap->pos = {(float) i * 5,0,0 };
			////スケール
			vertMap->scale = i + 1.f;
			vertMap->color = {1,1,1,1};

			//次の頂点へ
			vertMap++;
		}
		vertBuff_->Unmap(0, nullptr);
	}
}

void Particle::Draw(uint32_t descriptorSize)
{
	TextureManager::GetInstance()->
		SetGraphicsDescriptorTable(descriptorSize);

	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	//commandList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());
	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->DrawInstanced((UINT)activeCount_, 1, 0, 0);
}
