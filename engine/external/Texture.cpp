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
	//デスクリプタヒープの設定
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = (UINT)kMaxSRVCount;

	//設定をもとにSRV用でスクリプタヒープを生成
	result = device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	//SRVヒープの先頭ハンドルを取得
	srvHandle = srvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	
	//ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	textureHandle = 0;
}

uint32_t TextureManager::LoadGraph(const std::string& HandleName)
{
	HRESULT result = E_FAIL;
	uint32_t graphHandle{};
	//画像を格納するアドレスを代入
	graphHandle = textureHandle;
	//ユニークポインタで宣言
	std::unique_ptr<Texture> texture_ = std::make_unique<Texture>();
	//最終的なファイル名
	std::string allFileName;

	std::string find_Name = "Resources/";
	size_t strPos = HandleName.find(find_Name);
	//"Resources/"　が文字列の最初になければ文字列を足す
	
	if (strPos == 0)
	{
		allFileName = HandleName;
	}
	else
	{
		allFileName = "Resources/" + HandleName;
	}
#pragma region 画像読み込み
	//stringをwchar_tに変換
	wchar_t* fileName = ConvertStrToWchar(allFileName);

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	ScratchImage mipChain{};

	UINT descriptorSize = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//画像の名前を保存する
	texture_->handleName_ = HandleName;
	//同じ画像があった場合その画像と同じ数値を返す
	for (int i = 0; i < texData.size(); i++) {
		if (texData.size() > 0) {
			if (texData.at(i * descriptorSize)->handleName_ == HandleName) {
				return i * descriptorSize;
			}
		}
	}
	//ファイルの拡張子を代入
	std::string extension = FileExtension(HandleName);

	if (extension == "png")
	{
		//WICテクスチャダウンロード
		result = LoadFromWICFile(
			fileName,
			WIC_FLAGS_NONE,
			&metadata, scratchImg);
	}
	else if(extension == "tga")
	{
		//WICテクスチャダウンロード
		result = LoadFromTGAFile(
			fileName,
			&metadata, scratchImg);
	}
	else
	{
		
	}

	if (result != S_OK) {
		result = LoadFromWICFile(
			L"Resources/white1×1.png",
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
	texture_->textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texture_->textureResourceDesc.Format = metadata.format;
	texture_->textureResourceDesc.Width = metadata.width;
	texture_->textureResourceDesc.Height = (UINT)metadata.height;
	texture_->textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texture_->textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	texture_->textureResourceDesc.SampleDesc.Count = 1;
	//テクスチャバッファの生成
	result = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texture_->textureResourceDesc,
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
	texture_->srvDesc.Format = texture_->textureResourceDesc.Format;	//RGBA float
	texture_->srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	texture_->srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	texture_->srvDesc.Texture2D.MipLevels = texture_->textureResourceDesc.MipLevels;
	//ハンドルのさす位置にシェーダーリソースビュー作成
	device_->CreateShaderResourceView(texture_->texBuff.Get(), &texture_->srvDesc, srvHandle);
	//次に格納する場所のアドレスを示す
	textureHandle += descriptorSize;
	//次に格納する場所にアドレスを移す
	srvHandle.ptr += descriptorSize;

	//std::mapにHandleNameをキーワードにしたTexture型の配列を作る
	texData.insert(std::make_pair(graphHandle, std::move(texture_)));

	//画像を格納したアドレスを返す
	return graphHandle;
}

void TextureManager::SetGraphicsDescriptorTable(UINT descriptorSize)
{
	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* heaps[] = { srvHeap.Get() };
	DirectXCommon::GetInstance()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	srvGpuHandle = srvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	srvGpuHandle.ptr += descriptorSize;
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

D3D12_RESOURCE_DESC TextureManager::GetResDesc(UINT descriptorSize)
{
	return texData.at(descriptorSize).get()->textureResourceDesc;
}



#pragma endregion

// 拡張子を返す
std::string FileExtension(const std::string& path)
{
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}


