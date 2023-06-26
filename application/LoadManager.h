#pragma once
#include "Texture.h"
class LoadManager
{
private:

public:
	void LoadAllResources();

	void LoadModel();
	void LoadTexture();

	inline void LoadGraph(const std::string& fileName, const std::string& name) {
		TextureManager::GetInstance()->LoadGraph(fileName, name);
	}
};

