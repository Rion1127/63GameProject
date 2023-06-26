#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"


void SceneManager::Ini()
{
	//ビルド開始時のシーン
	Transition<GameScene>();
	

	
}

void SceneManager::Update()
{
	scurrentScene_->Update();

	
}

void SceneManager::Draw()
{
	//ゲーム描画
	scurrentScene_->Draw();
}

std::unique_ptr<EmptyScene> SceneManager::scurrentScene_ = nullptr;

