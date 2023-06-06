#pragma once
#include "WinAPI.h"
#include "DirectX.h"
class ImGuiManager
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ImGuiManager* Getinstance();
	//初期化
	void Init();
	
	void Finalize();

	void Begin();
	void End();
	void Draw();
private:
	WinAPI* winApi_ = nullptr;
	DirectXCommon* directX_ = nullptr;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

