#include "DirectionalLight.h"
#include <cassert>

DirectXCommon* DirectionalLight::directX = nullptr;

DirectionalLight* DirectionalLight::Create()
{
	DirectionalLight* instance = new DirectionalLight();

	instance->Init();

	return instance;
}

void DirectionalLight::StaticInit()
{
	DirectionalLight::directX = DirectXCommon::GetInstance();
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
	result = directX->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	

	TransferConstBuffer();
}

void DirectionalLight::Update()
{
	//�l�̕ύX���������������萔�΂����ɓ]������
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void DirectionalLight::Draw(UINT rootParameterIndex)
{
	directX->GetCommandList()->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
}

void DirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;
		constMap->lightColor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void DirectionalLight::SetLightDir(const Vector3& lightdir)
{
	//���K������
	this->lightdir = lightdir;
	this->lightdir.normalize();
	dirty = true;
}

void DirectionalLight::SetLightColor(const Vector3& color)
{
	this->lightcolor = color;
	dirty = true;
}
