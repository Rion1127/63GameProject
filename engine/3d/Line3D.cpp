#include "Line3D.h"
#include "Util.h"
#include "PipelineManager.h"
#include <cassert>

Line3D::Line3D(uint32_t vertsize)
{
	vertSize_ = vertsize;
	HRESULT result;
	vertex_.resize(vertSize_);
	UINT sizeVB =
		static_cast<UINT>(sizeof(VertexPos) * vertex_.size());

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

	constBuffColor_ = CreateBuff(constMapColor_);
}

void Line3D::Update()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPos* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	std::copy(vertex_.begin(), vertex_.end(), vertMap);

	constMapColor_->color = color_ / 255.f;

	transform_.Update();
}

void Line3D::Draw()
{
	PipelineManager::PreDraw("Line3D", LINESTRIP);

	auto& cmdList = *RDirectX::GetInstance()->GetCommandList();

	// 頂点データ転送
	cmdList.IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファ転送
	cmdList.SetGraphicsRootConstantBufferView(
		0, transform_.constBuffTransform_->GetGPUVirtualAddress());
	// 定数バッファ転送
	cmdList.SetGraphicsRootConstantBufferView(
		1, constBuffColor_->GetGPUVirtualAddress());

	cmdList.DrawInstanced((UINT)vertex_.size(), 1, 0, 0);
}

void Line3D::SetVertPos(const std::vector<Vector3>& pos)
{
	for (uint32_t i = 0; i < vertex_.size();i++) {
		if (pos.size() <= i) {
			if (vertex_[i].pos.x != pos[pos.size() - 1].x ||
				vertex_[i].pos.y != pos[pos.size() - 1].y ||
				vertex_[i].pos.z != pos[pos.size() - 1].z)
			{
				vertex_[i].pos = pos[pos.size() - 1];
			}
		}
		else {
			vertex_[i].pos = pos[i];
		}
	}
}
