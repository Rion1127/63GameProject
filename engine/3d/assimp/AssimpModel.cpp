#include "AssimpModel.h"
#include "Texture.h"
#include "DirectX.h"

/**
 * @file AssimpModel.cpp
 * @brief ボーンや頂点データを管理している
 */

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
