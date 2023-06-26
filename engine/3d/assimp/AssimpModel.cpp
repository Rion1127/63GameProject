#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

LightGroup* AssimpModel::slightGroup_ = nullptr;

AssimpModel::AssimpModel()
{
	constBuff_ = CreateBuff(constMap_);
}

void AssimpModel::Update()
{
	//定数バッファのマッピング
	
	for (uint32_t i = 0; i < importSetting_->boneData.size(); i++) {
		constMap_->bonesMatrix[i] = importSetting_->boneData[i].boneMatrix_;
	}
}

void AssimpModel::Create(const wchar_t* modelFile)
{
	ImportSettings importSetting = {
		modelFile,
		meshes_,
		false,
		true
	};

	importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));
	//assimpファイル読み込み
	AssimpLoader::GetInstance()->Load(importSetting_.get());

	texture_.resize(meshes_.size());
	materials_.resize(meshes_.size());
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer();

		std::string texturename = WStringToString(meshes_[i].diffuseMap);
		TextureManager::GetInstance()->LoadGraph(texturename, texturename);
		texture_[i] = *TextureManager::GetInstance()->GetTexture(texturename);

		//マテリアル生成
		//マテリアルの値を代入する（現在は適当な値を入れている）
		materials_[i].SetAmbient({ 0.8f,0.8f,0.8f });
		materials_[i].SetDiffuse({ 0.3f,0.3f,0.3f });
		materials_[i].SetSpecular({ 0.3f,0.3f,0.3f });
	}
}

void AssimpModel::Draw(const WorldTransform& WT)
{
	slightGroup_->Draw(3);
	//定数バッファビュー(CBV)の設定コマンド
	RDirectX::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(4, constBuff_->GetGPUVirtualAddress());
	for (uint32_t i = 0; i < importSetting_->meshes.size(); i++)
	{
		materials_[i].Draw(texture_[i].textureHandle);

		importSetting_->meshes[i].Vertices.Draw(WT, 0);
	}


}

