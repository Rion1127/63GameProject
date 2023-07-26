#include "ModelManager.h"

std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::models_;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadModel(const std::string& modelname, bool smoothing)
{
	std::unique_ptr<Model> instance = std::make_unique<Model>();
	instance = Model::CreateOBJ_uniptr(modelname, smoothing);

	models_.insert(std::make_pair(modelname, std::move(instance)));
}
