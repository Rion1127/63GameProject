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
	LoadGraph("white1280x720.dds", "White1280x720");
	LoadGraph("white1x1.dds", "White");
	LoadGraph("title.dds", "Title");
	//HPバー
	LoadGraph("hpBarBack.dds", "HpBarBack");
	LoadGraph("gauge.dds", "Gauge");
	//パーティクル
	LoadGraph("starParticle.dds", "StarParticle");
	LoadGraph("smoke.dds", "Smoke");
	LoadGraph("heart.dds", "Heart");
	LoadGraph("circle.dds", "Circle");
	LoadGraph("particle.dds", "Particle");
	LoadGraph("fireCircle.dds", "FireCircle");
	LoadGraph("explosion_01.dds", "Explosion_01");
	LoadGraph("explosion_02.dds", "Explosion_02");
	LoadGraph("swordTrail.dds", "SwordTrail");
	LoadGraph("hitCircle.dds", "HitCircle");
	LoadGraph("triangle.dds", "Triangle");
	LoadGraph("selectParticle.dds", "SelectParticle");
	LoadGraph("moveSmoke.dds", "MoveSmoke");
	LoadGraph("groundCrack.dds", "GroundCrack");
	//UI
	LoadGraph("LockOn1.dds", "LockOn1");
	LoadGraph("LockOn2.dds", "LockOn2");
	LoadGraph("ready.dds", "Ready");
	LoadGraph("go.dds", "Go");
	LoadGraph("clear.dds", "Clear");
	LoadGraph("readyShape.dds", "ReadyShape");
	LoadGraph("goShape.dds", "GoShape");
	LoadGraph("pause.dds", "Pause");
	LoadGraph("selectFrame.dds", "SelectFrame");
	LoadGraph("unselectFrame.dds", "UnselectFrame");
	LoadGraph("selectTex.dds", "SelectTex");
	LoadGraph("cameraInvTex.dds", "CameraInvTex");
	LoadGraph("configCameraTex.dds", "ConfigCameraTex");
	LoadGraph("bgmSeTex.dds", "BgmSeTex");
	LoadGraph("soundFrame.dds", "SoundFrame");
	LoadGraph("soundFrame.dds", "SoundFrame");

	LoadGraph("commandTitle.dds", "CommandTitle");
	LoadGraph("commandFrame.dds", "CommandFrame");
	LoadGraph("commandTex.dds", "CommandTex");
	LoadGraph("commandSpecialTex.dds", "CommandSpecialTex");
	LoadGraph("number.dds", "Number");
	LoadGraph("comboGaugeFrame.dds", "ComboGaugeFrame");
	LoadGraph("comboGauge.dds", "ComboGauge");
	
	//ボタンUI
	LoadGraph("pressA.dds", "PressA");
	LoadGraph("OperationUI.dds", "OperationUI");
	LoadGraph("button.dds", "Button");

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
