#pragma once
#include "WinAPI.h"
#include "DirectX.h"
class ImGuiManager
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ImGuiManager* Getinstance();
	//������
	void Init();
	
	void Finalize();

	void Begin();
	void End();
	void Draw();
private:
	WinAPI* winApi_ = nullptr;
	DirectXCommon* directX_ = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

