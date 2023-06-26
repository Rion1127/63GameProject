#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include "DirectXTex/d3dx12.h"
#include <cassert>
#include <vector>
#include <DirectXTex.h>
#include "Vector2.h"
using namespace DirectX;

struct Texture {
	//テクスチャバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	//テクスチャファイル名
	std::string fileName_;
	//設定したテクスチャの名前
	std::string textureName_;
	//テクスチャアドレス
	uint32_t textureHandle;
	//テクスチャサイズ
	Vector2 size_;
};

class TextureManager
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//テクスチャの情報を格納
	std::map < std::string , std::unique_ptr<Texture>> texData{};
	//次に格納する場所のアドレス
	uint32_t textureHandle;

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//設定をもとにSRV用でスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
public:
	static TextureManager* GetInstance();

	void Ini();

	void LoadGraph(const std::string& fileName, const std::string& name);

	void SetGraphicsDescriptorTable(UINT descriptorSize);

	Texture* GetTexture(const std::string& name);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
};

std::string FileExtension(const std::string& path);

