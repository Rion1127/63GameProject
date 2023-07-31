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
	LoadGraph("title.png", "Title");
	//HP�o�[
	LoadGraph("hpBarBack.png", "HpBarBack");
	LoadGraph("gauge.png", "Gauge");
	//�p�[�e�B�N��
	LoadGraph("starParticle.png", "StarParticle");
	LoadGraph("smoke.png", "Smoke");
	LoadGraph("heart.png", "Heart");
	LoadGraph("circle.png", "Circle");
	//UI
	LoadGraph("LockOn1.png", "LockOn1");
	LoadGraph("LockOn2.png", "LockOn2");
	LoadGraph("UItex.png", "UItex");

	//�{�^��UI
	LoadGraph("pressA.png", "PressA");
	LoadGraph("OperationUI.png", "OperationUI");
	//�Q�[���I�[�o�[UI
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
