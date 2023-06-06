#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <cassert>
#include <DirectXTex.h>
using namespace DirectX;
class TextureManager
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static TextureManager* GetInstance();

	void Ini(ID3D12Device* device);

	uint32_t LoadGraph(const std::string& HandleName);

	void SetGraphicsDescriptorTable(UINT descriptorSize);

	D3D12_RESOURCE_DESC GetResDesc(UINT descriptorSize);

	//設定をもとにSRV用でスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
private:
	struct Texture {
		//テクスチャ
		std::string handleName_;
		//リソース設定
		D3D12_RESOURCE_DESC textureResourceDesc{};
		//シェーダリソースビュー設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
		//テクスチャバッファの生成
		ComPtr<ID3D12Resource> texBuff = nullptr;

	};
	ComPtr<ID3D12Device> device_{};	//デバイスを格納
	//テクスチャの情報を格納
	std::map < uint32_t , std::unique_ptr<Texture>> texData{};
	////テクスチャバッファを格納
	//std::map <char, int> test;
	//画像を保存するアドレスを格納する
	uint32_t textureHandle;

//SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

};

std::string FileExtension(const std::string& path);

