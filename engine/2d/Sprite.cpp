#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <d3dx12.h>
#include <cassert>
#include <string>
#include "Texture.h"
#include <DirectXTex.h>
#include "Util.h"
#include "Sprite.h"
#include <sstream>
#include "myMath.h"

DirectXCommon* Sprite::directX_ = nullptr;
int Sprite::allNum = 0;

void Sprite::StaticIni()
{
	directX_ = DirectXCommon::GetInstance();
}

void Sprite::Ini(std::string guiname)
{
	spriteNum = allNum;
	Sprite::AddAllNum();




	std::string noneString = "";
	//何も入っていない場合
	if (guiname == noneString) {

		std::ostringstream oss;

		oss << spriteNum;
		name = "Sprite" + oss.str();
		gui = name.c_str();
	}
	else {
		guiName_ = guiname;

		gui = guiName_.c_str();
	}

	HRESULT result;
#pragma region 頂点データ
	//頂点データ
	vertices.push_back({ {   -0.0f,100.0f,0.0f },{0.0f,1.0f} });//左下
	vertices.push_back({ {   -0.0f, +0.0f,0.0f },{0.0f,0.0f} });//左上
	vertices.push_back({ { +100.0f,100.0f,0.0f },{1.0f,1.0f} });//右下
	vertices.push_back({ { +100.0f, +0.0f,0.0f },{1.0f,0.0f} });//右上

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
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

	result = directX_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

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
#pragma endregion

#pragma region インデックスデータ
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	result = directX_->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < indices.size(); i++) {
		indexMap[i] = indices[i]; // 座標をコピー
	}
	// 繋がりを解除
	indexBuff->Unmap(0, nullptr);
	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region シェーダーに色を渡す

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = directX_->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);
#pragma endregion

#pragma region トランスフォーム
	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = directX_->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));
	}

	// 定数バッファマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
	//単位行列を代入
	constMapTransform->mat = XMMatrixIdentity();

	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)1280, (float)720, 0.0f, 0.0f, 1.0f);

	//スケール
	Scale_ = { 1.f,1.f };
	color_ = { 1,1,1,1 };
	anchorPoint_ = { 0.5f,0.5f };
#pragma endregion
}

void Sprite::SetPos(Vector2 pos)
{
	pos_ = pos;
}

void Sprite::SetRot(float rot)
{
	rot_ = rot;
}

void Sprite::SetScale(Vector2 scale)
{
	Scale_ = scale;
}

void Sprite::DrawImGui()
{
	ImGui::Begin(gui);
	/* ここに追加したいGUIを書く */

	//static int clicked = 0;
	if (ImGui::Button("isInvisible_"))clicked++;
	//クリックされた場合表示しない
	if (clicked & 1)
	{
		ImGui::SameLine();
		ImGui::Text("TRUE");
		isInvisible_ = true;
	}
	else {
		ImGui::SameLine();
		ImGui::Text("FALSE");
		isInvisible_ = false;
	}

	// Menu Bar
	if (ImGui::CollapsingHeader("Posision"))
	{
		float x = pos_.x;
		float y = pos_.y;
		ImGui::SliderFloat("pos.x", &x, 0.0f, 2000.0f, "x = %.3f");
		ImGui::SliderFloat("pos.y", &y, 0.0f, 2000.0f, "y = %.3f");
		pos_.x = x;
		pos_.y = y;
	}
	// スケール変更
	if (ImGui::CollapsingHeader("Scale"))
	{
		float scalex = Scale_.x;
		float scaley = Scale_.y;
		ImGui::SliderFloat("scale.x", &scalex, 0.0f, 1.0f, "x = %.3f");
		ImGui::SliderFloat("scale.y", &scaley, 0.0f, 1.0f, "y = %.3f");
		Scale_.x = scalex;
		Scale_.y = scaley;
	}
	//回転
	if (ImGui::CollapsingHeader("Rotation"))
	{
		float rot = rot_;
		ImGui::SliderFloat("Rot", &rot, 0.0f, ConvertAngleToRadian(360), "x = %.3f");
		rot_ = rot;
	}

	ImGui::End();
}

void Sprite::PreDraw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(3)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(3)->GetRootSignature());

	// プリミティブ形状の設定コマンド
	directX_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void Sprite::Draw(UINT descriptorSize)
{
	if (isImguiDisplay) {
		DrawImGui();
	}

	if (isInvisible_) {
		return;
	}
	HRESULT result = S_OK;
#pragma region 画像のサイズを取得
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResDesc(descriptorSize);

#pragma endregion
	float left = (0.0f - anchorPoint_.x) * resDesc.Width;
	float right = (1.0f - anchorPoint_.x) * resDesc.Width;
	float top = (0.0f - anchorPoint_.y) * resDesc.Height;
	float bottom = (1.0f - anchorPoint_.y) * resDesc.Height;
	//左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

#pragma region 画像の頂点データを更新
	vertices.at(LB).pos = { left	, bottom	,0 };//左下
	vertices.at(LT).pos = { left	, top		,0 };//左上
	vertices.at(RB).pos = { right	, bottom	,0 };//右下
	vertices.at(RT).pos = { right	, top		,0 };//右上

	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// ワールド行列の更新
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixScaling(Scale_.x, Scale_.y, 0);
	matWorld_ *= XMMatrixRotationZ(rot_);
	matWorld_ *= XMMatrixTranslation(pos_.x, pos_.y, 0.0f);

	// 定数バッファにデータ転送
	constMapMaterial->color = color_;
	constMapTransform->mat = matWorld_ * matProjection; // 行列の合成

#pragma endregion

#pragma region 画像範囲指定
	//切り取り範囲がどちらも0の場合UV座標は変えない
	if (textureSize.x != 0 && textureSize.y != 0) {
		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize.y) / resDesc.Height;
		//頂点のUVに反映する
		vertices.at(LB).uv = { tex_left	, tex_bottom };//左下
		vertices.at(LT).uv = { tex_left	, tex_top };//左上
		vertices.at(RB).uv = { tex_right, tex_bottom };//右下
		vertices.at(RT).uv = { tex_right, tex_top };//右上
	}
#pragma endregion

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//定数バッファビュー(CBV)の設定コマンド
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 頂点バッファビューの設定コマンド
	directX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	directX_->GetCommandList()->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	directX_->GetCommandList()->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Sprite::Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize)
{
	HRESULT result = S_OK;

#pragma region 画像の頂点データを更新
	vertices.at(0).pos = { LdX,LdY,0 };//左下
	vertices.at(1).pos = { LuX,LuY,0 };//左上
	vertices.at(2).pos = { RdX,RdY,0 };//右下
	vertices.at(3).pos = { RuX,RuY,0 };//右上
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// ワールド行列の更新
	matWorld_ = XMMatrixIdentity();
	matWorld_ *= XMMatrixScaling(Scale_.x, Scale_.y, 0);
	matWorld_ *= XMMatrixRotationZ(rot_);
	matWorld_ *= XMMatrixTranslation(pos_.x, pos_.y, 0.0f);

	// 定数バッファにデータ転送
	constMapMaterial->color = color_;
	constMapTransform->mat = matProjection; // 行列の合成

#pragma endregion
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//定数バッファビュー(CBV)の設定コマンド
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// 頂点バッファビューの設定コマンド
	directX_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定コマンド
	directX_->GetCommandList()->IASetIndexBuffer(&ibView);
	//定数バッファビュー(CBV)の設定コマンド
	directX_->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	directX_->GetCommandList()->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Sprite::SetBlend(int blend)
{
	if (blend > 3) blend = 3;
	else if (blend < 0) blend = 0;
	// パイプラインステートとルートシグネチャの設定コマンド
	directX_->GetCommandList()->SetPipelineState(
		PipelineManager::GetSpritePipeline(blend)->gerPipelineState());

	directX_->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetSpritePipeline(blend)->GetRootSignature());
}
