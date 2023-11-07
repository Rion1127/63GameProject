#include "IParticle.h"
#include "DirectX.h"
#include "Util.h"

/**
 * @file IParticle.cpp
 * @brief インターフェースクラス
 * @brief パーティクルクラスに継承するクラス
 */

IParticle::~IParticle() {

}

void IParticle::Init(int32_t vertexCount)
{
	HRESULT result;
	vertices_.resize(vertexCount);
	UINT sizeVB = static_cast<UINT>(sizeof(VertexParticle) * vertices_.size());

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

	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_)
		);
	assert(SUCCEEDED(result));

	vertBuff_->SetName(L"PARTICLE VERT BUFF");

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	billBoard = BillBoard::None;
}

void IParticle::Update()
{
	if (particles_.size() > 0) {
		//end_frameを超えたら削除
		DeleteUpdate();
		//動き更新
		MoveUpdate();
	}
}

void IParticle::Draw()
{
	if (particles_.size() > 0) {
		auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

		// 頂点データ転送
		cmdList.IASetVertexBuffers(0, 1, &vbView_);

		// 定数バッファ転送
		cmdList.SetGraphicsRootConstantBufferView(
			1, transform_.constBuffTransform_->GetGPUVirtualAddress());

		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture.textureHandle);

		cmdList.DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()), 1, 0, 0);
	}
}

void IParticle::TransferBuff()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexParticle* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (int32_t i = 0; i < particles_.size(); i++)
	{
		vertices_.at(i).pos = particles_[i].position;

		vertices_.at(i).scale = particles_[i].scale;

		vertices_.at(i).rot = particles_[i].rot /** (float)Max(1 - isBillBoard ,0)*/;

		vertices_.at(i).ancorPoint = particles_[i].ancorPoint_;

		vertices_.at(i).color = particles_[i].color / 255.f;
		vertices_.at(i).color.a = particles_[i].color.a / 255.f;
	}
	std::copy(vertices_.begin(), vertices_.end(), vertMap);

	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);

	transform_.Update(billBoard);

}

void IParticle::DeleteUpdate()
{
	for (int32_t i = 0; i < particles_.size(); i++)
	{
		particles_[i].rate = (float)particles_[i].frame / (float)particles_[i].end_frame;

		if (particles_[i].frame >= particles_[i].end_frame)
		{
			particles_.erase(particles_.begin() + i);
			vertices_.at(i).scale = 0;
			i = -1;
		}
	}
}