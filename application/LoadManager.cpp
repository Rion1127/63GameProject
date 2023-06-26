#include "LoadManager.h"


void LoadManager::LoadAllResources()
{
	LoadTexture();
	LoadModel();
}

void LoadManager::LoadModel()
{
}

void LoadManager::LoadTexture()
{
	LoadGraph("white1Å~1.png", "White");
	LoadGraph("white1280x720.png", "White1280x720");
	LoadGraph("test.png", "Test");
	LoadGraph("uv.png", "uv");
	LoadGraph("keyBlade2.png", "key");
}
