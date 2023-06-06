#include <DirectXMath.h>
using namespace DirectX;

#include "DirectX.h"
#include "Vertices.h"

void Vertices::Ini(ID3D12Device* device)
{
	HRESULT result;
#pragma region 頂点データ
	//前
	//vertices.push_back({ {-5.0f,-5.0f, 0.0f },{}, { 0.0f,1.0f } });//左下
	//vertices.push_back({ {-5.0f, 5.0f, 0.0f },{}, { 0.0f,0.0f } });//左上
	//vertices.push_back({ { 5.0f,-5.0f, 0.0f },{}, { 1.0f,1.0f } });//右下
	//vertices.push_back({ { 5.0f, 5.0f, 0.0f },{}, { 1.0f,0.0f } });//右上
	vertices.push_back({ { 0.0f, 0.0f, 0.0f },{0,0,1}, { 0,1 } });//右上
	//後ろ
	//vertices.push_back({ {-5.0f,  5.0f, 5.0f},{}, { 0.0f,0.0f} });//左上
	//vertices.push_back({ {-5.0f, -5.0f, 5.0f},{}, { 0.0f,1.0f} });//左下
	//vertices.push_back({ { 5.0f,  5.0f, 5.0f},{}, { 1.0f,0.0f} });//右上
	//vertices.push_back({ { 5.0f, -5.0f, 5.0f},{}, { 1.0f,1.0f} });//右下
	////左
	//vertices.push_back({ {-5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//左下
	//vertices.push_back({ {-5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//左上
	//vertices.push_back({ {-5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//右下
	//vertices.push_back({ {-5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//右上
	////右				 
	//vertices.push_back({ {5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//左上
	//vertices.push_back({ {5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//左下
	//vertices.push_back({ {5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//右上
	//vertices.push_back({ {5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//右下
	////下				 
	//vertices.push_back({ { 5.0f,-5.0f,-5.0f} ,{},{0.0f,1.0f} });//左下
	//vertices.push_back({ { 5.0f,-5.0f, 5.0f} ,{},{0.0f,0.0f} });//左上
	//vertices.push_back({ {-5.0f,-5.0f,-5.0f} ,{},{1.0f,1.0f} });//右下
	//vertices.push_back({ {-5.0f,-5.0f, 5.0f} ,{},{1.0f,0.0f} });//右上
	////上				 
	//vertices.push_back({ { 5.0f, 5.0f, 5.0f} ,{},{0.0f,0.0f} });//左上
	//vertices.push_back({ { 5.0f, 5.0f,-5.0f} ,{},{0.0f,1.0f} });//左下
	//vertices.push_back({ {-5.0f, 5.0f, 5.0f} ,{},{1.0f,0.0f} });//右上
	//vertices.push_back({ {-5.0f, 5.0f,-5.0f} ,{},{1.0f,1.0f} });//右下
#pragma endregion
#pragma region 頂点インデックス
	//前
	/*indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);*/
	//後ろ
	//indices.push_back(4);
	//indices.push_back(5);
	//indices.push_back(6);
	//indices.push_back(6);
	//indices.push_back(5);
	//indices.push_back(7);
	////左
	//indices.push_back(8);
	//indices.push_back(9);
	//indices.push_back(10);
	//indices.push_back(10);
	//indices.push_back(9);
	//indices.push_back(11);
	////右
	//indices.push_back(12);
	//indices.push_back(13);
	//indices.push_back(14);
	//indices.push_back(14);
	//indices.push_back(13);
	//indices.push_back(15);
	////下
	//indices.push_back(16);
	//indices.push_back(17);
	//indices.push_back(18);
	//indices.push_back(18);
	//indices.push_back(17);
	//indices.push_back(19);
	////上
	//indices.push_back(20);
	//indices.push_back(21);
	//indices.push_back(22);
	//indices.push_back(22);
	//indices.push_back(21);
	//indices.push_back(23);
#pragma endregion

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;
	UINT sizeIB;
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

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
	for (int i = 0; i < indices.size() / 3; i++) {
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化（長さを1にする）
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);

	}

	// 頂点バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	if (indices.size() != 0) {
		//リソース設定
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		assert(SUCCEEDED(result));

		// インデックスバッファをマッピング
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// 全インデックスに対して
		for (int i = 0; i < indices.size(); i++) {
			indexMap[i] = indices[i]; // インデックスをコピー
		}
		// マッピング解除
		indexBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;
	}
}

void Vertices::CreateBuffer()
{
	HRESULT result;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());

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
	for (int i = 0; i < indices.size() / 3; i++) {
		//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化（長さを1にする）
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);

	}

	// 頂点バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;				// 頂点バッファのサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);	// 頂点1つ分のデータサイズ

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全インデックスに対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i]; // インデックスをコピー
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Vertices::Map()
{
	HRESULT result;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexPosNormalUv* vertMap = nullptr;
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void Vertices::Draw(uint32_t indexSize,
	WorldTransform* worldTransform,
	UINT descriptorSize)
{
	// 頂点バッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indexSize, 1, 0, 0, 0);
}

void Vertices::Draw(WorldTransform* worldTransform, UINT descriptorSize)
{
	// 頂点バッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Vertices::DrawInstanced(WorldTransform* worldTransform, UINT descriptorSize)
{
	// 頂点バッファビューの設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	//commandList->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, worldTransform->constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	DirectXCommon::GetInstance()->GetCommandList()->
		DrawInstanced((UINT)vertices.size(), 1, 0, 0);
}


