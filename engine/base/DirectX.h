#pragma once
#include <d3dcompiler.h>

#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <d3d12sdklayers.h>
#include "WinAPI.h"
#include <chrono>
class RDirectX
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	WinAPI* winApi_ = nullptr;
	// フェンスの生成
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
public:
	static RDirectX* GetInstance();
	//初期化
	void Ini(WinAPI* winApi);
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//レンダーターゲットのクリア
	void ClearRenderTarget();
	//深度バッファのクリア
	void ClearDepthBuffer();
private:
	RDirectX(){};
	//DXGIデバイス初期化
	void DXGIDeviceIni();
	//スワップチェーンの生成
	void CreateSwapChain();
	//コマンド関連初期化
	void CommandIni();
	//レンダーターゲット生成
	void CreateFinalRenderTargets();
	//深度バッファ生成
	void CreateDepthBuffer();
	///フェンス生成
	void CreateFence();
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
public:
	//ゲッター
	ID3D12Device* GetDevice() { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }
	//バックバッファの数を取得
	size_t GetBackBufferCount() { return backBuffers_.size(); }
};

