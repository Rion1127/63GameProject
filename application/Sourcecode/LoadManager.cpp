#include "LoadManager.h"

/**
 * @file LoadManager.cpp
 * @brief リソースを読み込みをまとめている
 */

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
	LoadGraph("particle.png", "Particle");
	LoadGraph("fireCircle.png", "FireCircle");
	LoadGraph("explosion_01.png", "Explosion_01");
	LoadGraph("explosion_02.png", "Explosion_02");
	LoadGraph("swordTrail.png", "SwordTrail");
	LoadGraph("hitCircle.png", "HitCircle");
	LoadGraph("triangle.png", "Triangle");
	LoadGraph("selectParticle.png", "SelectParticle");
	LoadGraph("moveSmoke.png", "MoveSmoke");
	LoadGraph("groundCrack.png", "GroundCrack");
	//UI
	LoadGraph("LockOn1.png", "LockOn1");
	LoadGraph("LockOn2.png", "LockOn2");
	LoadGraph("ready.png", "Ready");
	LoadGraph("go.png", "Go");
	LoadGraph("clear.png", "Clear");
	LoadGraph("readyShape.png", "ReadyShape");
	LoadGraph("goShape.png", "GoShape");
	LoadGraph("pause.png", "Pause");
	LoadGraph("selectFrame.png", "SelectFrame");
	LoadGraph("unselectFrame.png", "UnselectFrame");
	LoadGraph("selectTex.png", "SelectTex");
	LoadGraph("cameraInvTex.png", "CameraInvTex");
	LoadGraph("configCameraTex.png", "ConfigCameraTex");
	LoadGraph("bgmSeTex.png", "BgmSeTex");
	LoadGraph("soundFrame.png", "SoundFrame");
	LoadGraph("soundFrame.png", "SoundFrame");

	LoadGraph("commandTitle.png", "CommandTitle");
	LoadGraph("commandFrame.png", "CommandFrame");
	LoadGraph("commandTex.png", "CommandTex");
	LoadGraph("commandSpecialTex.png", "CommandSpecialTex");
	LoadGraph("number.png", "Number");
	LoadGraph("comboGaugeFrame.png", "ComboGaugeFrame");
	LoadGraph("comboGauge.png", "ComboGauge");
	
	//ボタンUI
	LoadGraph("pressA.png", "PressA");
	LoadGraph("OperationUI.png", "OperationUI");
	LoadGraph("button.png", "Button");

}

void LoadManager::LoadSound()
{
	LoadWave("swingSE.wav", "SwingSE");
	LoadWave("hitSE.wav", "HitSE");
	LoadWave("lockOnSE.wav", "lockOnSE");
	LoadWave("enterSE.wav", "EnterSE");
	LoadWave("selectSE.wav", "SelectSE");
	LoadWave("guardSE.wav", "GuardSE");
	LoadWave("guardHitSE.wav", "GuardHitSE");
	LoadWave("deathSE.wav", "DeathSE");
	LoadWave("shineSE.wav", "ShineSE");
	LoadWave("menuSE.wav", "MenuSE");
	LoadWave("fireSE.wav", "FireSE");
	LoadWave("roundClearSE.wav", "RoundClearSE");
	LoadWave("cancelSE.wav", "CancelSE");
	LoadWave("groundStumpSE.wav", "GroundStumpSE");
}

void LoadManager::LoadEnemyPopData()
{
	std::string path = "application/Resources/EnemyPOP/";
	EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "DebugEnemy.csv", "Debug");
	EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "HadesCup.csv", "HadesCup");
}
