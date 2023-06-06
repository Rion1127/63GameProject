#pragma once
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <d3dx12.h>
#pragma comment(lib, "d3d12.lib")
#include <wrl.h>
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")
#include <d3d12sdklayers.h>
#include "WinAPI.h"
#include <chrono>
class DirectXCommon
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectXCommon* GetInstance();

	ID3D12Device* GetDevice() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

	//バックバッファの数を取得
	size_t GetBackBufferCount() { return backBuffers.size(); }

	void Ini(WinAPI* winApi);

private:
	// DXGIデバイス初期化
	void DXGIDeviceIni();
	// スワップチェーンの生成
	void CreateSwapChain();
	// コマンド関連初期化
	void CommandIni();
	// レンダーターゲット生成
	void CreateFinalRenderTargets();
	// 深度バッファ生成
	void CreateDepthBuffer();
	/// フェンス生成
	void CreateFence();
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	WinAPI* winApi_ = nullptr;
	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
	
public:
	// 描画前処理
	void PreDraw();
	// 描画後処理
	void PostDraw();
	// レンダーターゲットのクリア
	void ClearRenderTarget();
	// 深度バッファのクリア
	void ClearDepthBuffer();

};

