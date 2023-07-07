#include "MultipleRenderTarget.h"
#include "WinAPI.h"
#include <d3dcompiler.h>

#include "DirectX.h"
#include "PipelineManager.h"
#include "mInput.h"


const float MultiRenderTarget::clearColor_[4] = { 0.25f,0.5f,0.1f,0.0f };
const uint32_t MultiRenderTarget::vertNum_ = 4;

MultiRenderTarget::MultiRenderTarget() /*:Sprite()*/
{
	//���_�o�b�t�@����
	CreateVertBuff();
	//ibView����
	CreateibView();
	//�萔�o�b�t�@����
	CreateConstBuff();
	//�e�N�X�`������
	CreateTexBuff();
	//SRV����
	CreateSRV();
	//RTV�쐬
	CreateRTV();
	//�[�x�o�b�t�@����
	CreateDepthBuff();
	//DSV�쐬
	CreateDSV();
}

void MultiRenderTarget::PUpdate()
{
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		Color color_(1, 1, 1, 1);
		// �萔�o�b�t�@�Ƀf�[�^�]��
		constMap->color = color_;
		constMap->mat.UnitMatrix(); // �s��̍���
	}

}

void MultiRenderTarget::Draw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->SetPipelineState(
		PipelineManager::GetPipelineObjects("PostEffect")->GetPipelineStateAlpha());

	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootSignature(
		PipelineManager::GetPipelineObjects("PostEffect")->GetRootSignature());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	if (Key::TriggerKey(DIK_V)) {
		//�f�X�N���v�^�q�[�v��SRV�쐬
		static uint32_t tex = 0;
		//�e�N�X�`���ԍ���0��1�Ő؂�ւ�
		tex = (tex == 0) ? 1 : 0;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		RDirectX::GetInstance()->GetDevice()->
			CreateShaderResourceView(texBuff_[tex].Get(),
				&srvDesc,
				descHeapSRV_->GetCPUDescriptorHandleForHeapStart());
	}



	//SRV�q�[�v�̐ݒ�R�}���h
	std::vector<ID3D12DescriptorHeap*> heaps = { descHeapSRV_.Get() };
	RDirectX::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps.data());
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = descHeapSRV_.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^0�Ԃɐݒ�
	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	RDirectX::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView_);

	//�`��R�}���h
	RDirectX::GetInstance()->GetCommandList()->
		DrawIndexedInstanced((UINT)indices_.size(), 1, 0, 0, 0);
}

void MultiRenderTarget::PreDrawScene()
{
	ID3D12GraphicsCommandList& cmdList = *RDirectX::GetInstance()->GetCommandList();
	for (int32_t i = 0; i < 2; i++) {
		auto bariier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff_[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		RDirectX::GetInstance()->GetCommandList()
			->ResourceBarrier(1,
				&bariier);
	}

	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2]{};
	for (int32_t i = 0; i < 2; i++) {
		rtvHs[i] =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), (INT)i,
				RDirectX::GetInstance()->GetDevice()->
				GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList.OMSetRenderTargets(2, rtvHs, false, &dsvH);
	//�r���[�|�[�g�̐ݒ�
	CD3DX12_VIEWPORT viewPorts[2]{};
	//�V�U�����O��`�̐ݒ�
	CD3DX12_RECT rects[2]{};
	for (int32_t i = 0; i < 2; i++) {
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f,
			WinAPI::GetWindowSize().x, WinAPI::GetWindowSize().y);
		rects[i] = CD3DX12_RECT(0, 0,
			(LONG)WinAPI::GetWindowSize().x, (LONG)WinAPI::GetWindowSize().y);
	}

	cmdList.RSSetViewports(2, viewPorts);
	cmdList.RSSetScissorRects(2, rects);

	//�S��ʃN���A
	for (int32_t i = 0; i < 2; i++) {
		cmdList.ClearRenderTargetView(rtvHs[i], clearColor_, 0, nullptr);
	}
	//�[�x�o�b�t�@�̃N���A
	cmdList.ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void MultiRenderTarget::PostDrawScene()
{
	//���\�[�X�o���A��ύX�i�`��\���V�F�[�_�[���\�[�X�j
	for (int32_t i = 0; i < 2; i++) {
		auto barrier =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff_[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		RDirectX::GetInstance()->GetCommandList()->
			ResourceBarrier(1, &barrier);
	}
}

void MultiRenderTarget::CreateVertBuff()
{
	HRESULT result;

	//���_�o�b�t�@����
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc =
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUV) * vertNum_);
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));
	//���_�f�[�^
	VertexPosUV vertices[vertNum_] = {
		{{-0.5f,-0.5f,0.0f},{0.f,1.f}},//����
		{{-0.5f,+0.5f,0.0f},{0.f,0.f}},//����
		{{+0.5f,-0.5f,0.0f},{1.f,1.f}},//�E��
		{{+0.5f,+0.5f,0.0f},{1.f,0.f}},//�E��
	};
	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff_->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̐ݒ�
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUV) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUV);
}

void MultiRenderTarget::CreateibView()
{
	HRESULT result;
	indices_.push_back(0);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(1);
	indices_.push_back(2);
	indices_.push_back(3);

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());

	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (uint32_t i = 0; i < indices_.size(); i++) {
		indexMap[i] = indices_[i]; // ���W���R�s�[
	}
	// �q���������
	indexBuff_->Unmap(0, nullptr);
	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

void MultiRenderTarget::CreateConstBuff()
{
	HRESULT result;
	//�萔�o�b�t�@�̐���
	D3D12_HEAP_PROPERTIES heapProp =
		D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void MultiRenderTarget::CreateTexBuff()
{
	HRESULT result;

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		(UINT64)WinAPI::GetWindowSize().x,
		(UINT)WinAPI::GetWindowSize().y,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES prop =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);
	CD3DX12_CLEAR_VALUE clear_Value =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor_);
	for (int32_t i = 0; i < 2; i++) {
		//�e�N�X�`���o�b�t�@�̐���
		result =
			RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
				&prop,
				D3D12_HEAP_FLAG_NONE,
				&texResDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clear_Value,
				IID_PPV_ARGS(&texBuff_[i]));
		assert(SUCCEEDED(result));

		//��f��
		const size_t pixelCount = (size_t)WinAPI::GetWindowSize().x * (size_t)WinAPI::GetWindowSize().y;
		//�摜��s���̃f�[�^�T�C�Y
		const auto rowPitch = sizeof(UINT) * WinAPI::GetWindowSize().x;
		//�摜�S�̂̃f�[�^�T�C�Y
		const auto depthPitch = rowPitch * WinAPI::GetWindowSize().y;
		//�摜�C���[�W
		std::vector<UINT> img;
		img.resize(pixelCount);
		for (size_t j = 0; j < pixelCount; j++)
		{
			img[j] = 0xff0000ff;
		}
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff_[i]->WriteToSubresource(0, nullptr,
			img.data(), (UINT)rowPitch, (UINT)depthPitch);
		assert(SUCCEEDED(result));
	}
}

void MultiRenderTarget::CreateSRV()
{
	HRESULT result;
	//SRV�p�ŃX�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	result = RDirectX::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));
	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	//�f�X�N���v�^�q�[�v��SRV�쐬
	RDirectX::GetInstance()->GetDevice()->
		CreateShaderResourceView(texBuff_[0].Get(),
			&srvDesc,
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart()
		);
}

void MultiRenderTarget::CreateRTV()
{
	HRESULT result;
	//RTV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = RDirectX::GetInstance()->GetDevice()
		->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));
	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	for (int32_t i = 0; i < 2; i++) {
		//�f�X�N���v�^�q�[�v��RTV�쐬
		RDirectX::GetInstance()->GetDevice()
			->CreateRenderTargetView(texBuff_[i].Get(),
				nullptr,
				CD3DX12_CPU_DESCRIPTOR_HANDLE(
					descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), (INT)i,
					RDirectX::GetInstance()->GetDevice()->
					GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))
			);
	}

}

void MultiRenderTarget::CreateDepthBuff()
{
	HRESULT result;
	CD3DX12_RESOURCE_DESC depthResDesc =
		//�[�x�o�b�t�@���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			(UINT64)WinAPI::GetWindowSize().x,
			(UINT)WinAPI::GetWindowSize().y,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//�[�x�o�b�t�@�̐���
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&depthBuff_));
	assert(SUCCEEDED(result));
}

void MultiRenderTarget::CreateDSV()
{
	HRESULT result;
	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = RDirectX::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	RDirectX::GetInstance()->GetDevice()->
		CreateDepthStencilView(depthBuff_.Get(),
			&dsvDesc,
			descHeapDSV_->GetCPUDescriptorHandleForHeapStart());
}
