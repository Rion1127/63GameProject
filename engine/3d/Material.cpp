#include <DirectXTex.h>
using namespace DirectX;
#include "Texture.h"
#include "Material.h"
#include "DirectX.h"

Material::Material()
{
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBuffMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMat_));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBufferMat_->Map(0, nullptr, (void**)&constMapMat_);	//マッピング
	assert(SUCCEEDED(result));
	constMapMat_->ambient = ambient_;
	constMapMat_->diffuse = diffuse_;
	constMapMat_->specular = specular_;
	constMapMat_->alpha = alpha_;
}

void Material::LoadTexture(const std::string& directoryPath)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// ファイルパスを結合
	std::string filepath = directoryPath + textureFilename_;
	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadGraph(filepath, filepath);
	texture_ = *TextureManager::GetInstance()->GetTexture(filepath);
}

void Material::Draw(UINT descriptorSize)
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//ルートパラメータ配列2番目を指定
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBufferMat_->GetGPUVirtualAddress());
}
