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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	//�o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	ComPtr<ID3D12Resource> depthBuffer_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	WinAPI* winApi_ = nullptr;
	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
	//�L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;
public:
	static RDirectX* GetInstance();
	//������
	void Ini(WinAPI* winApi);
	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();
	//�����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget();
	//�[�x�o�b�t�@�̃N���A
	void ClearDepthBuffer();
private:
	RDirectX(){};
	//DXGI�f�o�C�X������
	void DXGIDeviceIni();
	//�X���b�v�`�F�[���̐���
	void CreateSwapChain();
	//�R�}���h�֘A������
	void CommandIni();
	//�����_�[�^�[�Q�b�g����
	void CreateFinalRenderTargets();
	//�[�x�o�b�t�@����
	void CreateDepthBuffer();
	///�t�F���X����
	void CreateFence();
	//FPS�Œ菉����
	void InitializeFixFPS();
	//FPS�Œ�X�V
	void UpdateFixFPS();
public:
	//�Q�b�^�[
	ID3D12Device* GetDevice() { return device_.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }
	//�o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() { return backBuffers_.size(); }
};

