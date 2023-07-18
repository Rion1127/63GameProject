#include <d3dcompiler.h>

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

uint32_t Sprite::SAllNum = 0;

void Sprite::Ini(const std::string& guiname)
{
	spriteNum_ = SAllNum;
	Sprite::AddAllNum();

	std::string noneString = "";
	//何も入っていない場合
	if (guiname == noneString) {

		std::ostringstream oss;

		oss << spriteNum_;
		name_ = "Sprite" + oss.str();
		gui_ = name_.c_str();
	}
	else {
		guiName_ = guiname;

		gui_ = guiName_.c_str();
	}

	HRESULT result;
#pragma region 頂点データ
	//頂点データ
	vertices_.push_back({ {   -0.0f,100.0f,0.0f },{0.0f,1.0f} });//左下
	vertices_.push_back({ {   -0.0f, +0.0f,0.0f },{0.0f,0.0f} });//左上
	vertices_.push_back({ { +100.0f,100.0f,0.0f },{1.0f,1.0f} });//右下
	vertices_.push_back({ { +100.0f, +0.0f,0.0f },{1.0f,0.0f} });//右上

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
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

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i]; // 座標をコピー
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
#pragma endregion

#pragma region インデックスデータ
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(3);

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	indexBuff_->Unmap(0, nullptr);
	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;

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

	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);
	assert(SUCCEEDED(result));
	//値を書き込むと自動的に転送される
	constMapMaterial_->color = Color(1, 1, 1, 1.0f);
#pragma endregion

#pragma region トランスフォーム
	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result));
	}

	// 定数バッファマッピング
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result));
	//単位行列を代入
	constMapTransform_->mat.UnitMatrix();

	// 射影行列計算
	matProjection_ = {
		2 / WinAPI::GetWindowSize().x,0,0,0,
		0,-2 / WinAPI::GetWindowSize().y,0,0,
		0,0,1,0,
		-1,1,0,1
	};
	//スケール
	Scale_ = { 1.f,1.f };
	anchorPoint_ = { 0.5f,0.5f };
#pragma endregion
}

void Sprite::DrawImGui()
{
	ImGui::Begin(gui_);
	/* ここに追加したいGUIを書く */

	//static int clicked_ = 0;
	if (ImGui::Button("isInvisible_"))clicked_++;
	//クリックされた場合表示しない
	if (clicked_ & 1)
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
		ImGui::DragFloat("scale.x", &scalex,1.0f, 0.0f, 10.0f);
		ImGui::DragFloat("scale.y", &scaley,1.0f, 0.0f, 10.0f);
		Scale_.x = scalex;
		Scale_.y = scaley;
	}
	//回転
	if (ImGui::CollapsingHeader("Rotation"))
	{
		float rot = rot_;
		ImGui::SliderFloat("Rot", &rot, 0.0f, Radian(360), "x = %.3f");
		rot_ = rot;
	}

	ImGui::End();
}

void Sprite::Update()
{
	if (isInvisible_)
	{
		return;
	}

	// ワールド行列の更新
	matWorld_.UnitMatrix();
	matWorld_ *= ConvertScalingMat({ Scale_.x, Scale_.y, 0 });
	matWorld_ *= ConvertRotationZAxisMat(rot_);
	matWorld_ *= ConvertTranslationMat({ pos_.x, pos_.y, 0.0f });

	// 定数バッファにデータ転送
	constMapMaterial_->color = color_ / 255.f;
	constMapTransform_->mat = matWorld_ * matProjection_; // 行列の合成

#pragma endregion

	TransferVertex();
}

void Sprite::Draw()
{
	if (isImguiDisplay_) {
		DrawImGui();
	}

	if (isInvisible_) {
		return;
	}

	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuffMaterial_->GetGPUVirtualAddress());
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Sprite::Draw(float LuX, float LuY, float RuX, float RuY, float LdX, float LdY, float RdX, float RdY, UINT descriptorSize)
{
	HRESULT result = S_OK;

#pragma region 画像の頂点データを更新
	vertices_.at(0).pos = { LdX,LdY,0 };//左下
	vertices_.at(1).pos = { LuX,LuY,0 };//左上
	vertices_.at(2).pos = { RdX,RdY,0 };//右下
	vertices_.at(3).pos = { RuX,RuY,0 };//右上
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);

	// ワールド行列の更新
	matWorld_.UnitMatrix();
	matWorld_ *= ConvertScalingMat({ Scale_.x, Scale_.y, 0 });
	matWorld_ *= ConvertRotationZAxisMat(rot_);
	matWorld_ *= ConvertTranslationMat({ pos_.x, pos_.y, 0.0f });

	// 定数バッファにデータ転送
	constMapMaterial_->color = color_;
	constMapTransform_->mat = matProjection_; // 行列の合成

#pragma endregion
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	// 頂点バッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//インデックスバッファビューの設定コマンド
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void Sprite::SetBlend(uint32_t blend)
{
	//if (blend > 3) blend = 3;
	//else if (blend < 0) blend = 0;
	//// パイプラインステートとルートシグネチャの設定コマンド
	//RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
	//	PipelineManager::GetSpritePipeline(blend)->gerPipelineState());

	//RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
	//	PipelineManager::GetSpritePipeline(blend)->GetRootSignature());
}

void Sprite::TransferVertex()
{
	float left = (0.0f - anchorPoint_.x) * texture_.size_.x;
	float right = (1.0f - anchorPoint_.x) * texture_.size_.x;
	float top = (0.0f - anchorPoint_.y) * texture_.size_.y;
	float bottom = (1.0f - anchorPoint_.y) * texture_.size_.y;
	//左右反転
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices_.at(LB).pos = { left	, bottom	,0 };//左下
	vertices_.at(LT).pos = { left	, top		,0 };//左上
	vertices_.at(RB).pos = { right	, bottom	,0 };//右下
	vertices_.at(RT).pos = { right	, top		,0 };//右上

	std::copy(std::begin(vertices_), std::end(vertices_), vertMap_);

	//切り取り範囲がどちらも0の場合UV座標は変えない
	if (textureSize_.x != 0 && textureSize_.y != 0)
	{
		float tex_left = textureLeftTop_.x / texture_.size_.x;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / texture_.size_.x;
		float tex_top = textureLeftTop_.y / texture_.size_.y;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / texture_.size_.y;
		//頂点のUVに反映する
		vertices_.at(LB).uv = { tex_left	, tex_bottom };//左下
		vertices_.at(LT).uv = { tex_left	, tex_top };//左上
		vertices_.at(RB).uv = { tex_right, tex_bottom };//右下
		vertices_.at(RT).uv = { tex_right, tex_top };//右上
	}
}
