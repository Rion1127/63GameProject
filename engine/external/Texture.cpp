#include "Util.h"
#include "Texture.h"

#pragma region TextureManager


TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return &instance;
}

void TextureManager::Ini(ID3D12Device* device)
{
	device_ = device;
	HRESULT result;
	//�f�X�N���v�^�q�[�v�̐ݒ�
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = (UINT)kMaxSRVCount;

	//�ݒ�����Ƃ�SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//SRV�q�[�v�̐擪�n���h�����擾
	srvHandle = srvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	
	//�q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	textureHandle = 0;
}

uint32_t TextureManager::LoadGraph(const std::string& HandleName)
{
	HRESULT result = E_FAIL;
	uint32_t graphHandle{};
	//�摜���i�[����A�h���X����
	graphHandle = textureHandle;
	//���j�[�N�|�C���^�Ő錾
	std::unique_ptr<Texture> texture_ = std::make_unique<Texture>();
	//�ŏI�I�ȃt�@�C����
	std::string allFileName;

	std::string find_Name = "Resources/";
	size_t strPos = HandleName.find(find_Name);
	//"Resources/"�@��������̍ŏ��ɂȂ���Ε�����𑫂�
	
	if (strPos == 0)
	{
		allFileName = HandleName;
	}
	else
	{
		allFileName = "Resources/" + HandleName;
	}
#pragma region �摜�ǂݍ���
	//string��wchar_t�ɕϊ�
	wchar_t* fileName = ConvertStrToWchar(allFileName);

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//�摜�̖��O��ۑ�����
	texture_->handleName_ = HandleName;
	//�����摜���������ꍇ���̉摜�Ɠ������l��Ԃ�
	for (int i = 0; i < texData.size(); i++) {
		if (texData.size() > 0) {
			if (texData.at(i * descriptorSize)->handleName_ == HandleName) {
				return i * descriptorSize;
			}
		}
	}
	//�t�@�C���̊g���q����
	std::string extension = FileExtension(HandleName);

	if (extension == "png")
	{
		//WIC�e�N�X�`���_�E�����[�h
		result = LoadFromWICFile(
			fileName,
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}
	else if(extension == "tga")
	{
		//WIC�e�N�X�`���_�E�����[�h
		result = LoadFromTGAFile(
			fileName,
			&metadata, scratchImg);
	}
	else
	{
		
	}

	if (result != S_OK) {
		result = LoadFromWICFile(
			L"Resources/white1�~1.png",
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
	texture_->textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texture_->textureResourceDesc.Format = metadata.format;
	texture_->textureResourceDesc.Width = metadata.width;
	texture_->textureResourceDesc.Height = (UINT)metadata.height;
	texture_->textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texture_->textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	texture_->textureResourceDesc.SampleDesc.Count = 1;
	//�e�N�X�`���o�b�t�@�̐���
	result = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texture_->textureResourceDesc,
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
	texture_->srvDesc.Format = texture_->textureResourceDesc.Format;	//RGBA float
	texture_->srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	texture_->srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	texture_->srvDesc.Texture2D.MipLevels = texture_->textureResourceDesc.MipLevels;
	//�n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device_->CreateShaderResourceView(texture_->texBuff.Get(), &texture_->srvDesc, srvHandle);
	//���Ɋi�[����ꏊ�̃A�h���X������
	textureHandle += descriptorSize;
	//���Ɋi�[����ꏊ�ɃA�h���X���ڂ�
	srvHandle.ptr += descriptorSize;

	//std::map��HandleName���L�[���[�h�ɂ���Texture�^�̔z������
	texData.insert(std::make_pair(graphHandle, std::move(texture_)));

	//�摜���i�[�����A�h���X��Ԃ�
	return graphHandle;
}

void TextureManager::SetGraphicsDescriptorTable(UINT descriptorSize)
{
	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* heaps[] = { srvHeap.Get() };
	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	srvGpuHandle.ptr += descriptorSize;
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

D3D12_RESOURCE_DESC TextureManager::GetResDesc(UINT descriptorSize)
{
	return texData.at(descriptorSize).get()->textureResourceDesc;
}



#pragma endregion

// �g���q��Ԃ�
std::string FileExtension(const std::string& path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}


