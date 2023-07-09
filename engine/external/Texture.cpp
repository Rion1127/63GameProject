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
	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = (UINT)kMaxSRVCount;

	//設定をもとにSRV用でスクリプタヒープを生成
	result = device.
		CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//SRVヒープの先頭ハンドルを取得
	srvHandle = srvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	

	textureHandle = 0;
}

void TextureManager::LoadGraph(const std::string& fileName, const std::string& name)
{
	auto& device = *RDirectX::GetInstance()->GetDevice();
	HRESULT result = E_FAIL;
	uint32_t graphHandle{};
	//画像を格納するアドレスを代入
	graphHandle = textureHandle;
	//ユニークポインタで宣言
	std::unique_ptr<Texture> texture_ = std::make_unique<Texture>();
	//最終的なファイル名
	std::string allFileName;

	std::string find_Name = "application/Resources/";
	size_t strPos = fileName.find(find_Name);
	//"Resources/"　が文字列の最初になければ文字列を足す
	if (strPos == 0)
	{
		allFileName = fileName;
	}
	else
	{
		allFileName = find_Name + fileName;
	}
#pragma region 画像読み込み
	//stringをwchar_tに変換
	std::wstring allFileName_w = ToWideString(allFileName);

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	UINT descriptorSize = device.
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//画像の名前を保存する
	texture_->fileName_ = fileName;
	//同じ画像があった場合その画像と同じ数値を返す
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

	//ファイルの拡張子を代入
	std::string extension = FileExtension(fileName);

	if (extension == "png")
	{
		//WICテクスチャダウンロード
		result = LoadFromWICFile(
			allFileName_w.c_str(),
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}
	else if (extension == "tga")
	{
		//WICテクスチャダウンロード
		result = LoadFromTGAFile(
			allFileName_w.c_str(),
			&metadata, scratchImg);
	}
	
	//失敗したときは白のテクスチャを読み込む
	if (result != S_OK) {
		result = LoadFromWICFile(
			L"application/Resources/white1x1.png",
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}

	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);
#pragma endregion
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	//リソース設定
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	//ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//テクスチャバッファの生成
	result = device.CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//D3D12_RESOURCE_STATE_COPY_DESTに直せ！！
		nullptr,
		IID_PPV_ARGS(&texture_->texBuff));
	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texture_->texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch);	//1枚サイズ
		assert(SUCCEEDED(result));
	}
	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//シェーダリソースビュー設定
	srvDesc.Format = textureResourceDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
	//ハンドルのさす位置にシェーダーリソースビュー作成
	device.
		CreateShaderResourceView(texture_->texBuff.Get(), &srvDesc, srvHandle);
	//次に格納する場所のアドレスを示す
	textureHandle += descriptorSize;
	//次に格納する場所にアドレスを移す
	srvHandle.ptr += descriptorSize;

	texture_->textureHandle = graphHandle;
	texture_->textureName_ = name;
	texture_->size_ = {
		(float)metadata.width,
		(float)metadata.height,
	};

	//std::mapにHandleNameをキーワードにしたTexture型の配列を作る
	texData.insert(std::make_pair(name, std::move(texture_)));

}

void TextureManager::SetGraphicsDescriptorTable(UINT descriptorSize)
{
	//SRVヒープの設定コマンド
	std::vector<ID3D12DescriptorHeap*> heaps = { srvHeap.Get() };
	RDirectX::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps.data());
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
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

// 拡張子を返す
std::string FileExtension(const std::string& path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}


