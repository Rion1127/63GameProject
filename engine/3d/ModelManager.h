#pragma once
#include "Model.h"
#include <unordered_map>
class ModelManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<Model>> models_;
public:
	static ModelManager* GetInstance();

	Model* GetModel(std::string modelName) { return models_[modelName].get(); }
	//���f��������(CreateOBJ()�ɓ����Ă���)
	static void LoadModel(const std::string& modelname, bool smoothing);
private:
	ModelManager() {};

};

