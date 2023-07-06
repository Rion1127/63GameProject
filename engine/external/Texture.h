#pragma once
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <d3d12.h>

#include "DirectXTex/d3dx12.h"
#include <cassert>
#include <vector>
#include <DirectXTex.h>
#include "Vector2.h"

struct Texture {
	//�e�N�X�`���o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;
	//�e�N�X�`���t�@�C����
	std::string fileName_;
	//�ݒ肵���e�N�X�`���̖��O
	std::string textureName_;
	//�e�N�X�`���A�h���X
	uint32_t textureHandle;
	//�e�N�X�`���T�C�Y
	Vector2 size_;
};

class TextureManager
{
private:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�e�N�X�`���̏����i�[
	std::map < std::string , std::unique_ptr<Texture>> texData{};
	//���Ɋi�[����ꏊ�̃A�h���X
	uint32_t textureHandle;

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;
	
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle{};
	//�ݒ�����Ƃ�SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap{};
public:
	static TextureManager* GetInstance();

	void Ini();

	void LoadGraph(const std::string& fileName, const std::string& name);

	void SetGraphicsDescriptorTable(UINT descriptorSize);

	Texture* GetTexture(const std::string& name);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
private:
	TextureManager() {};
};

std::string FileExtension(const std::string& path);

