#include "LoadManager.h"


void LoadManager::LoadAllResources()
{
	LoadTexture();
	LoadModel();
	LoadSound();
	LoadEnemyPopData();
}

void LoadManager::LoadModel()
{
}

void LoadManager::LoadTexture()
{
	LoadGraph("white1280x720.png", "White1280x720");
	LoadGraph("white1x1.png", "White");
	LoadGraph("title.png", "Title");
	//HPバー
	LoadGraph("hpBarBack.png", "HpBarBack");
	LoadGraph("gauge.png", "Gauge");
	//パーティクル
	LoadGraph("starParticle.png", "StarParticle");
	LoadGraph("smoke.png", "Smoke");
	LoadGraph("heart.png", "Heart");
	LoadGraph("circle.png", "Circle");
	//UI
	LoadGraph("LockOn1.png", "LockOn1");
	LoadGraph("LockOn2.png", "LockOn2");
	LoadGraph("UItex.png", "UItex");
	LoadGraph("ready.png", "Ready");
	LoadGraph("go.png", "Go");
	LoadGraph("clear.png", "Clear");
	LoadGraph("retry.png", "Retry");
	LoadGraph("readyShape.png", "ReadyShape");
	LoadGraph("goShape.png", "GoShape");

	//ボタンUI
	LoadGraph("pressA.png", "PressA");
	LoadGraph("OperationUI.png", "OperationUI");
	//ゲームオーバーUI
	LoadGraph("continueTex.png", "Continue");
	LoadGraph("TitleTex.png", "TitleTex");

}

void LoadManager::LoadSound()
{
	LoadWave("swingSE.wav", "SwingSE");
	LoadWave("hitSE.wav", "HitSE");
	LoadWave("lockOnSE.wav", "lockOnSE");
	LoadWave("enterSE.wav", "EnterSE");
	LoadWave("selectSE.wav", "SelectSE");
	LoadWave("guardSE.wav", "Guard");
	LoadWave("guardHitSE.wav", "GuardHitSE");
	LoadWave("deathSE.wav", "DeathSE");
}

void LoadManager::LoadEnemyPopData()
{
	std::string path = "application/Resources/EnemyPOP/";
	EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "DebugEnemy.csv", "Debug");
	EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "HadesCup.csv", "HadesCup");
	EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "KingCup.csv","KingCup");
}
