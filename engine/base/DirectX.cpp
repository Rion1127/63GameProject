#include <algorithm>
#include <cassert>
#include <vector>
#include <thread>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include "DirectX.h"

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Ini(WinAPI* winApi)
{
	winApi_ = winApi;
	//FPS�Œ菉����
	InitializeFixFPS();
	// DXGI�f�o�C�X������
	DXGIDeviceIni();
	// �R�}���h�֘A������
	CommandIni();
	// �X���b�v�`�F�[���̐���
	CreateSwapChain();
	// �����_�[�^�[�Q�b�g����
	CreateFinalRenderTargets();
	// �[�x�o�b�t�@����
	CreateDepthBuffer();
	// �t�F���X����
	CreateFence();
}

void DirectXCommon::DXGIDeviceIni() {
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}

	// DRED���|�[�g���I����
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dredSettings)))) {
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
#endif

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
	  D3D_FEATURE_LEVEL_12_1,
	  D3D_FEATURE_LEVEL_12_0,
	  D3D_FEATURE_LEVEL_11_1,
	  D3D_FEATURE_LEVEL_11_0,
	};

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) !=
		DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

	// �n�[�h�E�F�A�A�_�v�^��D��I�ɏ���
	std::stable_sort(
		adapters.begin(), adapters.end(),
		[](const ComPtr<IDXGIAdapter4>& lhs, const ComPtr<IDXGIAdapter4>& rhs) {
			DXGI_ADAPTER_DESC3 lhsDesc;
			lhs->GetDesc3(&lhsDesc); // �A�_�v�^�[�̏����擾
			DXGI_ADAPTER_DESC3 rhsDesc;
			rhs->GetDesc3(&rhsDesc); // �A�_�v�^�[�̏����擾
			return (lhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) <
				(rhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE);
		});

	result = S_FALSE;
	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < adapters.size(); i++) {
		// �f�o�C�X�𐶐�
		for (int levelIndex = 0; levelIndex < _countof(levels); levelIndex++) {
			result =
				D3D12CreateDevice(adapters[i].Get(), levels[levelIndex], IID_PPV_ARGS(&device));
			if (SUCCEEDED(result)) {
				// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
				featureLevel = levels[levelIndex];
				break;
			}
		}

		// ���̃A�_�v�^�Ő����ł��Ă��犮��
		if (SUCCEEDED(result)) {
			break;
		}
	}

	assert(!!device);
	assert(SUCCEEDED(result));

}

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;

	//�X���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = winApi_->win_width;
	swapChainDesc.Height = winApi_->win_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �F���̏�������ʓI�Ȃ��̂�
	swapChainDesc.SampleDesc.Count = 1;					// �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// �o�b�N�o�b�t�@�Ƃ��Ďg����悤��
	swapChainDesc.BufferCount = 2;						// �o�b�t�@�����Q�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͑��₩�ɔj��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // �e�B�A�����O�T�|�[�g
	ComPtr<IDXGISwapChain1> swapChain1;
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApi_->hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain�𓾂�
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	assert(SUCCEEDED(result));

	
}

void DirectXCommon::CommandIni() {
	HRESULT result = S_FALSE;

	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	// �R�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// �}������G���[
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// �}������\�����x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// �w�肵���G���[�̕\����}������
		infoQueue->PushStorageFilter(&filter);
		
	}
#endif
}

void DirectXCommon::CreateFinalRenderTargets() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapChain->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

	// �f�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(result));

	// ���\�̂Q���ɂ���
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		assert(SUCCEEDED(result));

		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			rtvHeap->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &renderTargetViewDesc, handle);
	}
}

void DirectXCommon::CreateDepthBuffer() {
	HRESULT result = S_FALSE;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, 1280, 720, 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	// ���\�[�X�̐���
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
		&clearValue, IID_PPV_ARGS(&depthBuffer_));
	assert(SUCCEEDED(result));

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;                    // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	assert(SUCCEEDED(result));

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer_.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	// �t�F���X�̐���
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));
}


void DirectXCommon::InitializeFixFPS()
{
	//���ݎ��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//�O��L�^����̌o�܎��Ԃ��擾����
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60�i���킸���ɒZ�����ԁj�����Ă��Ȃ�����
	if (elapsed < kMinCheckTime) {
		// 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::PreDraw() {
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// ���\�[�X�o���A��ύX�i�\����ԁ��`��Ώہj
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &barrier);

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// �����_�[�^�[�Q�b�g���Z�b�g
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �S��ʃN���A
	ClearRenderTarget();
	// �[�x�o�b�t�@�N���A
	ClearDepthBuffer();

	// �r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT viewport =
		CD3DX12_VIEWPORT(0.0f, 0.0f, 1280, 720);
	commandList->RSSetViewports(1, &viewport);
	// �V�U�����O��`�̐ݒ�
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, 1280, 720);
	commandList->RSSetScissorRects(1, &rect);
}

void DirectXCommon::PostDraw() {
	HRESULT result;

	// ���\�[�X�o���A��ύX�i�`��Ώہ��\����ԁj
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &barrier);

	// ���߂̃N���[�Y
	commandList->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { commandList.Get() }; // �R�}���h���X�g�̔z��
	commandQueue_->ExecuteCommandLists(1, cmdLists);

	// �o�b�t�@���t���b�v
	result = swapChain->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = device->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// �����p�������擾
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	// �R�}���h���X�g�̎��s������҂�
	commandQueue_->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS�Œ�
	UpdateFixFPS();

	commandAllocator_->Reset(); // �L���[���N���A
	commandList->Reset(commandAllocator_.Get(),
		nullptr); // �ĂуR�}���h���X�g�𒙂߂鏀��
}

void DirectXCommon::ClearRenderTarget() {
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeap->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	float black[] = {0,0,0,0};
	float blue[] = { 0.1f, 0.25f, 0.5f, 0.0f };

	// �S��ʃN���A        Red   Green Blue  Alpha
	//float clearColor[] = { 0,0,0,0/*0.1f, 0.25f, 0.5f, 0.0f */}; // ���ۂ��F
	commandList->ClearRenderTargetView(rtvH, blue, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer() {
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
