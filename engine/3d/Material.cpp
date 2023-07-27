#include <DirectXTex.h>
#include "Texture.h"
#include "Material.h"
#include "DirectX.h"
using namespace DirectX;

Material::Material()
{
	HRESULT result;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBuffMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = RDirectX::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,		//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMat_));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBufferMat_->Map(0, nullptr, (void**)&constMapMat_);	//�}�b�s���O
	assert(SUCCEEDED(result));
	constMapMat_->ambient = ambient_;
	constMapMat_->diffuse = diffuse_;
	constMapMat_->specular = specular_;
	constMapMat_->alpha = alpha_;
}

void Material::LoadTexture(const std::string& directoryPath)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// �t�@�C���p�X������
	std::string filepath = "application/Resources/Object/" + directoryPath + textureFilename_;
	// �e�N�X�`���ǂݍ���
	TextureManager::GetInstance()->LoadGraph(filepath, filepath);
	texture_ = *TextureManager::GetInstance()->GetTexture(filepath);
}

void Material::Draw(UINT descriptorSize)
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(descriptorSize);
	//���[�g�p�����[�^�z��2�Ԗڂ��w��
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBufferMat_->GetGPUVirtualAddress());
}

void Material::Draw()
{
	TextureManager::GetInstance()->SetGraphicsDescriptorTable(texture_.textureHandle);
	//���[�g�p�����[�^�z��2�Ԗڂ��w��
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(2, constBufferMat_->GetGPUVirtualAddress());
}
