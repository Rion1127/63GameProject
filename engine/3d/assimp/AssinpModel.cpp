#include "AssinpModel.h"
#include "Texture.h"
#include "DirectX.h"

AssinpModel::AssinpModel()
{
}

void AssinpModel::Create(const wchar_t* modelFile)
{
	ImportSettings importSetting = {
		modelFile,
		meshes,
		false,
		true
	};

	importSetting_ = std::move(std::make_unique<ImportSettings>(importSetting));

	AssimpLoader::GetInstance()->Load(importSetting_.get());

	texHandle_.resize(meshes.size());
	for (int i = 0; i < importSetting_->meshes.size(); i++)
	{
		importSetting_->meshes[i].Vertices.CreateBuffer();

		std::string texturename = WStringToString(meshes[i].diffuseMap);
		texHandle_[i] = TextureManager::GetInstance()->LoadGraph(texturename);
	}
}

void AssinpModel::Draw(WorldTransform WT)
{
	for (int i = 0; i < importSetting_->meshes.size(); i++)
	{
		TextureManager::GetInstance()->
			SetGraphicsDescriptorTable(texHandle_[i]);

		importSetting_->meshes[i].Vertices.Draw(&WT, 0);
	}
}

