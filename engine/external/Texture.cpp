#include "Util.h"
#include "Texture.h"
using namespace DirectX;

#pragma region TextureManager


TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Ini()
{
	auto& device = *RDirectX::GetInstance()->GetDevice();
	HRESULT result;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = (UINT)kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	result = device.
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap.Get()->GetCPUDescriptorHandleForHeapStart();


	textureHandle = 0;
}

void TextureManager::LoadGraph(const std::string& fileName, const std::string& name)
{
	auto& device = *RDirectX::GetInstance()->GetDevice();
	HRESULT result = E_FAIL;
	uint32_t graphHandle{};
	//�摜���i�[����A�h���X����
	graphHandle = textureHandle;
	//���j�[�N�|�C���^�Ő錾
	std::unique_ptr<Texture> texture_ = std::make_unique<Texture>();
	//�ŏI�I�ȃt�@�C����
	std::string allFileName;

	std::string find_Name = "Resources/";
	size_t strPos = fileName.find(find_Name);
	size_t strSize = fileName.size();
	//"Resources/"�@��������̍ŏ��ɂȂ���Ε�����𑫂�
	if (strPos <= strSize)
	{
		allFileName = fileName;
	}
	else
	{
		allFileName = "application/Resources/Texture/" + fileName;
	}
#pragma region �摜�ǂݍ���
	//string��wchar_t�ɕϊ�
	std::wstring allFileName_w = ToWideString(allFileName);

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	UINT descriptorSize = device.
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�摜�̖��O��ۑ�����
	texture_->fileName_ = fileName;
	//�����摜���������ꍇ���̉摜�Ɠ������l��Ԃ�
	for (uint32_t i = 0; i < texData.size(); i++) {
		if (name == "") {
			continue;
			if (texData.size() > 0) {
				if (texData.find(name)->second->fileName_ == fileName) {
					break;
				}
			}
		}
	}

	//�t�@�C���̊g���q����
	std::string extension = FileExtension(fileName);

	if (extension == "png")
	{
		//WIC�e�N�X�`���_�E�����[�h
		result = LoadFromWICFile(
			allFileName_w.c_str(),
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}
	else if (extension == "tga")
	{
		//WIC�e�N�X�`���_�E�����[�h
		result = LoadFromTGAFile(
			allFileName_w.c_str(),
			&metadata, scratchImg);
	}

	//���s�����Ƃ��͔��̃e�N�X�`����ǂݍ���
	if (result != S_OK) {
		result = LoadFromWICFile(
			L"application/Resources/Texture/white1x1.png",
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}

	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
#pragma endregion
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	//���\�[�X�ݒ�
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//�e�N�X�`���o�b�t�@�̐���
	result = device.CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//D3D12_RESOURCE_STATE_COPY_DEST�ɒ����I�I
		nullptr,
		IID_PPV_ARGS(&texture_->texBuff));
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texture_->texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//�S�̈�փR�s�[
			img->pixels,			//���f�[�^�A�h���X
			(UINT)img->rowPitch,	//1���C���T�C�Y
			(UINT)img->slicePitch);	//1���T�C�Y
		assert(SUCCEEDED(result));
	}
	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//�ݒ�\����
	//�V�F�[�_���\�[�X�r���[�ݒ�
	srvDesc.Format = textureResourceDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
	//�n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device.
		CreateShaderResourceView(texture_->texBuff.Get(), &srvDesc, srvHandle);
	//���Ɋi�[����ꏊ�̃A�h���X������
	textureHandle += descriptorSize;
	//���Ɋi�[����ꏊ�ɃA�h���X���ڂ�
	srvHandle.ptr += descriptorSize;

	texture_->textureHandle = graphHandle;
	texture_->textureName_ = name;
	texture_->size_ = {
		(float)metadata.width,
		(float)metadata.height,
	};

	//std::map��HandleName���L�[���[�h�ɂ���Texture�^�̔z������
	texData.insert(std::make_pair(name, std::move(texture_)));

}

void TextureManager::SetGraphicsDescriptorTable(UINT descriptorSize)
{
	//SRV�q�[�v�̐ݒ�R�}���h
	std::vector<ID3D12DescriptorHeap*> heaps = { srvHeap.Get() };
	RDirectX::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps.data());
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	srvGpuHandle.ptr += descriptorSize;
	RDirectX::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvGpuHandle);
}

Texture* TextureManager::GetTexture(const std::string& name)
{
	return texData[name].get();
}

ID3D12Resource* TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	auto& device = *RDirectX::GetInstance()->GetDevice();
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(&device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	//ID3D12Resource* intermediateResource =
	return nullptr;
}



#pragma endregion

// �g���q��Ԃ�
std::string FileExtension(const std::string& path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}


