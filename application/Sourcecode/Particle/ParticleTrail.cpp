#include "ParticleTrail.h"
#include <cassert>
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>
#include "Util.h"
#include "Spline.h"

/**
 * @file ParticleTrail.cpp
 * @brief 軌道エフェクトを作るクラス
 */

ParticleTrail::ParticleTrail(uint32_t vertSize) :
	vertSize_(vertSize)
{
	HRESULT result;
	vertex_.resize(vertSize * 2);
	posArray_.resize(vertSize);
	UINT sizeVB =
		static_cast<UINT>(sizeof(SwordTrailVertex) * vertex_.size());

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

	vertBuff_->SetName(L"SWORDTRAIL VERT BUFF");

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertex_[0]);

	isStop_ = true;
	isVisible_ = false;

	constBuffColor_ = CreateBuff(constMapColor_);
}

void ParticleTrail::Update()
{
	
	if (isStop_) {
		//先頭の値を配列の後ろへ代入していく
		for (size_t i = posArray_.size() - 1; i > 0; --i)
		{
			posArray_[i] = posArray_[i - 1];
		}

		//配列の先頭に現在の座標を代入
		posArray_.front() = tempPos;
		tempPos = PosBuffer();
	}

	////曲線を作る
	//std::vector<PosBuffer> usedPosArray = posArray_;
	//if (usedPosArray.empty())return;
	//CreateCurveVertex(usedPosArray);

	TransferBuff();

	transform_.Update();
}

void ParticleTrail::SetPos(const Vector3& head, const Vector3& tail)
{
	tempPos.head = head;
	tempPos.tail = tail;
}

void ParticleTrail::Draw()
{
	if (isVisible_) {
		PipelineManager::PreDraw("SwordTrail", TRIANGLESTRIP);

		auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

		// 頂点データ転送
		cmdList.IASetVertexBuffers(0, 1, &vbView_);

		// 定数バッファ転送
		cmdList.SetGraphicsRootConstantBufferView(
			1, transform_.constBuffTransform_->GetGPUVirtualAddress());
		// 定数バッファ転送
		cmdList.SetGraphicsRootConstantBufferView(
			2, constBuffColor_->GetGPUVirtualAddress());

		// テクスチャ
		TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);

		cmdList.DrawInstanced((UINT)std::distance(vertex_.begin(), vertex_.end()), 1, 0, 0);
	}
}

void ParticleTrail::DrawImgui()
{
	ImGui::Begin("swordTrail");

	if (ImGui::Button("Reset")) {
		posArray_.clear();
	}
	if (ImGui::Button("whiteTexture")) {
		texture_ = *TextureManager::GetInstance()->GetTexture("White");
	}
	if (ImGui::Button("particleTexture")) {
		texture_ = *TextureManager::GetInstance()->GetTexture("Circle");
	}
	if (ImGui::Button("Stop")) {
		isStop_ = (isStop_ == true) ? false : true;
	}
	ImGui::End();
}

void ParticleTrail::TransferBuff()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	SwordTrailVertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//頂点データを更新する
	float amount = 1.0f / (posArray_.size() - 1);
	float v = 0;
	vertex_.clear();
	vertex_.resize(posArray_.size() * 2);
	for (size_t i = 0, j = 0; i < vertex_.size() && j < posArray_.size(); i += 2, ++j)
	{
		//頂点座標を二つ代入する
		vertex_[i].pos = posArray_[j].head;
		vertex_[i].uv = Vector2(1.0f, v);
		vertex_[i + 1].pos = posArray_[j].tail;
		vertex_[i + 1].uv = Vector2(0.0f, v);


		v += amount;
	}
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		if (i > 0) {
			if (vertex_[i].pos.x == 0 &&
				vertex_[i].pos.y == 0 &&
				vertex_[i].pos.z == 0)
			{
				vertex_[i].pos = vertex_[i - 1].pos;
			}
		}
	}
	std::copy(vertex_.begin(), vertex_.end(), vertMap);

	constMapColor_->color = color_ / 255.f;
}

void ParticleTrail::CreateCurveVertex(std::vector<PosBuffer>& usedPosArray)
{
	usedPosArray;
}
