#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

AssimpModel::AssimpModel()
{
}

void AssimpModel::CreateModel(std::string fileName)
{
}

void AssimpModel::PlayAnimetion()
{
}

void AssimpModel::Draw(WorldTransform WT)
{
	for (auto& m : materials_) {
		m->Draw();
	}
	for (auto& v : vertices_) {
		v->Draw(WT);
	}
}
