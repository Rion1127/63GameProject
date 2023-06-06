#include <DirectXTex.h>
using namespace DirectX;
#include "Texture.h"
#include "Material.h"
#include "DirectX.h"

Material* Material::Create(ID3D12Device* device)
{
	Material* instance = new Material;

	instance->Ini(device);

	return instance;
}

void Material::Ini(ID3D12Device* device)
{
	HRESULT result;
	////�q�[�v�ݒ�
	//D3D12_HEAP_PROPERTIES cbHeapProp{};
	//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC cbResourceDesc{};
	//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	//cbResourceDesc.Height = 1;
	//cbResourceDesc.DepthOrArraySize = 1;
	//cbResourceDesc.MipLevels = 1;
	//cbResourceDesc.SampleDesc.Count = 1;
	//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////�萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&cbHeapProp,		//�q�[�v�ݒ�
	//	D3D12_HEAP_FLAG_NONE,
	//	&cbResourceDesc,	//���\�[�X�ݒ�
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBufferMaterial));
	//assert(SUCCEEDED(result));
	////�萔�o�b�t�@�̃}�b�s���O
	//result = constBufferMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	//assert(SUCCEEDED(result));
	////�l���������ނƎ����I�ɓ]�������
	//constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);

	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB3) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//�萔�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&cbHeapProp,		//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,	//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBufferB3));
		assert(SUCCEEDED(result));
		//�萔�o�b�t�@�̃}�b�s���O
		result = constBufferB3->Map(0, nullptr, (void**)&constMapB3);	//�}�b�s���O
		assert(SUCCEEDED(result));
		constMapB3->ambient = ambient_;
		constMapB3->diffuse = diffuse_;
		constMapB3->specular = specular_;
		constMapB3->alpha = alpha;
		//constBufferB3->Unmap(0,nullptr);
	}
}

void Material::ChangeColor(float x, float y, float z, float w)
{
	//�l���������ނƎ����I�ɓ]�������
	//constMapMaterial->color = XMFLOAT4(x,y,z,w);
}

void Material::LoadTexture(const std::string& directoryPath)
{

	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	std::string filepath = directoryPath + textureFilename_;

	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::GetInstance()->LoadGraph(filepath);
}

void Material::ChangeColor(XMFLOAT4 color_)
{
	//�l���������ނƎ����I�ɓ]�������
	//constMapMaterial->color = XMFLOAT4(color_);
}

void Material::Draw(UINT descriptorSize)
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
	//���[�g�p�����[�^�z��2�Ԗڂ��w��
	DirectXCommon::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBufferB3->GetGPUVirtualAddress());
}
