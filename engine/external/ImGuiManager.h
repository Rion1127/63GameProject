#pragma once
#include "WinAPI.h"
#include "DirectX.h"
class ImGuiManager
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	WinAPI* winApi_ = nullptr;
	RDirectX* directX_ = nullptr;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
public:
	static ImGuiManager* Getinstance();
	//初期化
	void Init();
	
	void Finalize();

	void Begin();
	void End();
	void Draw();
private:
	ImGuiManager() {};
};

