#pragma once
#include "Texture.h"
#include "mSound.h"
class LoadManager
{
private:

public:
	void LoadAllResources();

	void LoadModel();
	void LoadTexture();
	void LoadSound();

	inline void LoadGraph(const std::string& fileName, const std::string& name) {
		TextureManager::GetInstance()->LoadGraph(fileName, name);
	}
	inline void LoadWave(const std::string& fileName, const std::string& name) {
		SoundManager::LoadWave(fileName, name);
	}
};

