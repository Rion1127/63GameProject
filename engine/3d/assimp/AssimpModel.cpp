#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

AssimpModel::AssimpModel()
{
}

void AssimpModel::CreateModel(const std::string& fileName)
{
	fileName;
}

void AssimpModel::PlayAnimetion()
{
}

void AssimpModel::Draw(const WorldTransform& WT)
{
	for (auto& m : materials_) {
		m->Draw();
	}
	for (auto& v : vertices_) {
		v->Draw(WT);
	}
}
