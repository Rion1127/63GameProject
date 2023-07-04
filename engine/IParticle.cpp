#include "IParticle.h"
#include "DirectX.h"
IParticle::IParticle()
{

}
IParticle::~IParticle() {
}

void IParticle::Init(int vertexCount)
{
	HRESULT result;

	vertices_.resize(vertexCount);
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexParticle) * vertices_.size());

	// 頂点バッファの設定
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexParticle* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (uint32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;				// 頂点バッファのサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);	// 頂点1つ分のデータサイズ
}

void IParticle::Update()
{
	//end_frameを超えたら削除
	DeleteUpdate();
	//動き更新
	MoveUpdate();
	//データ転送
	TransferBuff();
}

void IParticle::Draw()
{
	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

	// 頂点データ転送
	cmdList.IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファ転送
	cmdList.SetGraphicsRootConstantBufferView(
		0, transform.constBuffTransform_->GetGPUVirtualAddress());

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture.textureHandle);

	cmdList.DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void IParticle::TransferBuff()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexParticle* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (size_t i = 0; i < particles.size(); i++)
	{
		vertMap->pos = particles[i].position;

		vertMap->scale = particles[i].scale;

		vertMap->rot = particles[i].rot;

		vertMap->ancorPoint = particles[i].ancorPoint_;

		vertMap->color = particles[i].color /*/ 255.0f*/;
		vertMap->color.a = particles[i].color.a / 255.0f;

		vertices_.at(i) = *vertMap;
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	transform.Update();

}

void IParticle::DeleteUpdate()
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i].frame >= particles[i].end_frame)
		{
			particles.erase(particles.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}
}
