#include "DirectionalLight.h"
#include <cassert>


DirectionalLight* DirectionalLight::Create()
{
	DirectionalLight* instance = new DirectionalLight();

	instance->Init();

	return instance;
}

void DirectionalLight::StaticInit()
{
}

void DirectionalLight::Init()
{
	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	HRESULT result;
	// �萔�o�b�t�@�̐���
	result = RDirectX::GetInstance()->GetDevice()->
		CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	

	TransferConstBuffer();
}

void DirectionalLight::Update()
{
	//�l�̕ύX���������������萔�΂����ɓ]������
	if (dirty_) {
		TransferConstBuffer();
		dirty_ = false;
	}
}

void DirectionalLight::Draw(UINT rootParameterIndex)
{
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(3, constBuff_->GetGPUVirtualAddress());
}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	if (SUCCEEDED(result)) {
		constMap_->lightv = -lightdir_;
		constMap_->lightColor = lightcolor_;
		constBuff_->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const Vector3& lightdir)
{
	//���K������
	lightdir_ = lightdir;
	lightdir_ = lightdir_.normalize();
	dirty_ = true;
}

void DirectionalLight::SetLightColor(const Vector3& color)
{
	this->lightcolor_ = color;
	dirty_ = true;
}
