#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"
#include "myMath.h"
#include <vector>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class MultiRenderTarget
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Resource> texBuff_[2];
	ComPtr<ID3D12DescriptorHeap> descHeapSRV_;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff_;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;

	//頂点データ
	ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//インデックスデータ
	std::vector<uint16_t> indices_;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};
	//定数バッファ用データ構造体
	struct ConstBufferData {
		Color color;
		Matrix4 mat;
	};
	ComPtr<ID3D12Resource> constBuff_ = nullptr;
private:
	//画面クリアカラー
	static const float clearColor_[4];
public:
	MultiRenderTarget();

	void PUpdate();

	void Draw();
	//シーン前処理
	void PreDrawScene();
	//シーン描画後処理
	void PostDrawScene();
private:
	void CreateVertBuff();
	void CreateibView();
	void CreateConstBuff();
	void CreateTexBuff();
	void CreateSRV();
	void CreateRTV();
	void CreateDepthBuff();
	void CreateDSV();
public:
private:
	static const uint32_t vertNum_;
	struct VertexPosUV {
		Vector3 pos;
		Vector2 uv;
	};
};

