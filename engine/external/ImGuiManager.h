#pragma once
#include "WinAPI.h"
#include "DirectX.h"
class ImGuiManager
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	WinAPI* winApi_ = nullptr;
	RDirectX* directX_ = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
public:
	static ImGuiManager* Getinstance();
	//������
	void Init();
	
	void Finalize();

	void Begin();
	void End();
	void Draw();
private:
	ImGuiManager() {};
};

