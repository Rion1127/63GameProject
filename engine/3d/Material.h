#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <string>
#pragma comment(lib, "d3d12.lib")
#include <cassert>

class Material
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	static Material* Create(ID3D12Device* device);
	void Ini(ID3D12Device* device);

	void LoadTexture(const std::string& directoryPath);

	void SetAmbient(XMFLOAT3 Ambient) { ambient_ = Ambient;}
	void SetDiffuse(XMFLOAT3 Diffuse) { diffuse_ = Diffuse;}
	void SetSpecular(XMFLOAT3 Specular) { specular_ = Specular;}

	void ChangeColor(float x, float y, float z, float w);
	void ChangeColor(XMFLOAT4 color_);

	void Draw(UINT descriptorSize);

	std::string name_;            // マテリアル名
	std::string textureFilename_; // テクスチャファイル名
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
private:
	////定数バッファ用データ構造体
	//struct ConstBufferDataMaterial {
	//	XMFLOAT4 color;
	//};
	////定数バッファの生成
	//ComPtr < ID3D12Resource> constBufferMaterial = nullptr;
	////定数バッファのマッピング
	//ConstBufferDataMaterial* constMapMaterial = nullptr;

	//定数バッファ用データ構造体
	struct ConstBufferDataB3 {
		XMFLOAT3 ambient = { 0.3f,0.3f,0.3f };
		float pad1;
		XMFLOAT3 diffuse = { 0.0f,0.0f,0.0f };
		float pad2;
		XMFLOAT3 specular = { 0.0f, 0.0f, 0.0f };
		float alpha;
	};
	//定数バッファの生成
	ComPtr < ID3D12Resource> constBufferB3 = nullptr;
	//定数バッファのマッピング
	ConstBufferDataB3* constMapB3 = nullptr;

	XMFLOAT3 ambient_ ;
	XMFLOAT3 diffuse_ ;
	XMFLOAT3 specular_ ;
	float alpha = 1.0f;
	
};

