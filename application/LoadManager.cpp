#include "LoadManager.h"


void LoadManager::LoadAllResources()
{
	LoadTexture();
	LoadModel();
	LoadSound();
}

void LoadManager::LoadModel()
{
}

void LoadManager::LoadTexture()
{
	LoadGraph("white1280x720.png", "White1280x720");
	LoadGraph("white1x1.png", "White");
	LoadGraph("test.png", "Test");
	LoadGraph("uv.png", "uv");
	//HPバー
	LoadGraph("hpBarBack.png", "HpBarBack");
	LoadGraph("gauge.png", "Gauge");
	//パーティクル
	LoadGraph("starParticle.png", "StarParticle");
}

void LoadManager::LoadSound()
{
	LoadWave("swingSE.wav", "SwingSE");
	LoadWave("hitSE.wav", "HitSE");
}
