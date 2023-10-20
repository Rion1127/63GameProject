#include <DirectXMath.h>

#include "DirectX.h"
#include "Vertices.h"

/**
 * @file Vertices.cpp
 * @brief 頂点データを管理しているクラス
 */

void Vertices::Ini()
{
	HRESULT result;

	vertices_.push_back({ { 0.0f, 0.0f, 0.0f },{0,0,1}, { 0,1 } });//右上

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;
	UINT sizeIB;
	sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

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

	//法線の計算
	for (uint32_t i = 0; i < indices_.size() / 3; i++) {
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 0];
		unsigned short index1 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 1];
		unsigned short index2 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		Vector3 normal = v1.cross(v2);
		//正規化（長さを1にする）
		normal = normal.normalize();
		//求めた法線を頂点データに代入
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;

	}

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
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	if (indices_.size() != 0) {
		//リソース設定
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
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
			IID_PPV_ARGS(&indexBuff_));
		assert(SUCCEEDED(result));

		// インデックスバッファをマッピング
		uint16_t* indexMap = nullptr;
		result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// 全インデックスに対して
		for (uint32_t i = 0; i < indices_.size(); i++) {
			indexMap[i] = indices_[i]; // インデックスをコピー
		}
		// マッピング解除
		indexBuff_->Unmap(0, nullptr);

		ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		ibView_.Format = DXGI_FORMAT_R16_UINT;
		ibView_.SizeInBytes = sizeIB;
	}
}

void Vertices::CreateBuffer()
{
	HRESULT result;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices_.size());

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

	//法線の計算
	for (uint32_t i = 0; i < indices_.size() / 3; i++) {
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 0];
		unsigned short index1 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 1];
		unsigned short index2 = indices_[static_cast<std::vector<uint16_t, std::allocator<uint16_t>>::size_type>(i) * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		Vector3 p0 = vertices_[index0].pos;
		Vector3 p1 = vertices_[index1].pos;
		Vector3 p2 = vertices_[index2].pos;
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		Vector3 v1 = p1 - p0;
		Vector3 v2 = p2 - p0;
		//外積は両方から垂直なベクトル
		Vector3 normal = v1.cross(v2);
		//正規化（長さを1にする）
		normal = normal.normalize();
		//求めた法線を頂点データに代入
		vertices_[index0].normal = normal;
		vertices_[index1].normal = normal;
		vertices_[index2].normal = normal;
	}

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
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeVB;				// 頂点バッファのサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);	// 頂点1つ分のデータサイズ

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
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
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全インデックスに対して
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // インデックスをコピー
	}
	// マッピング解除
	indexBuff_->Unmap(0, nullptr);

	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

}

void Vertices::Map()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void Vertices::Draw(uint32_t indexSize,
	WorldTransform* worldTransform)
{
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		IASetIndexBuffer(&ibView_);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform_->GetGPUVirtualAddress());
	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indexSize, 1, 0, 0, 0);
}

void Vertices::Draw(const WorldTransform& worldTransform)
{
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, worldTransform.constBuffTransform_->GetGPUVirtualAddress());
	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Vertices::DrawInstanced(WorldTransform* worldTransform)
{
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	//commandList->IASetIndexBuffer(&ibView_);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform_->GetGPUVirtualAddress());
	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawInstanced((UINT)vertices_.size(), 1, 0, 0);
}


