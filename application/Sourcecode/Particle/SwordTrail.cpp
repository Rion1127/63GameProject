#include "SwordTrail.h"
#include <cassert>
#include "DirectX.h"
#include "PipelineManager.h"
#include <imgui.h>

SwordTrail::SwordTrail(uint32_t vertSize) :
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

	timer_.SetLimitTime(10);
	texture_ = *TextureManager::GetInstance()->GetTexture("Circle");
}

void SwordTrail::Update()
{
	if (isStop_) {
		//データを更新
		for (size_t i = posArray_.size() - 1; i > 0; --i)
		{
			posArray_[i] = posArray_[i - 1];
		}
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

void SwordTrail::SetPos(const Vector3& head, const Vector3& tail)
{
	tempPos.head = head;
	tempPos.tail = tail;

	isTransfer_ = true;
}

void SwordTrail::Draw()
{
	PipelineManager::PreDraw("SwordTrail", TRIANGLESTRIP);


	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

	// 頂点データ転送
	cmdList.IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファ転送
	cmdList.SetGraphicsRootConstantBufferView(
		1, transform_.constBuffTransform_->GetGPUVirtualAddress());

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);

	cmdList.DrawInstanced((UINT)std::distance(vertex_.begin(), vertex_.end()), 1, 0, 0);


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

void SwordTrail::TransferBuff()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	SwordTrailVertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//頂点データを更新する
	float amount = 1.0f / (/*usedPosArray*/posArray_.size() - 1);
	float v = 0;
	vertex_.clear();
	vertex_.resize(posArray_/*usedPosArray*/.size() * 2);
	for (size_t i = 0, j = 0; i < vertex_.size() && j < posArray_/*usedPosArray*/.size(); i += 2, ++j)
	{
		vertex_[i].pos = posArray_/*usedPosArray*/[j].head;
		vertex_[i].uv = Vector2(1.0f, v);
		vertex_[i + 1].pos = posArray_/*usedPosArray*/[j].tail;
		vertex_[i + 1].uv = Vector2(0.0f, v);
		v += amount;
	}
	std::copy(vertex_.begin(), vertex_.end(), vertMap);
}
