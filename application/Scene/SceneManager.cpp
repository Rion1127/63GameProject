#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"
#include "DebugScene.h"
#include "TitleScene.h"

std::unique_ptr<EmptyScene> SceneManager::scurrentScene_ = nullptr;
SceneName SceneManager::ssceneName_;
bool SceneManager::sisSetNext_ = false;
Timer SceneManager::animeTimer_;

void SceneManager::Ini()
{
	//ビルド開始時のシーン
	Transition<TitleScene>();
	

	animeTimer_.SetLimitTime(120);
}

void SceneManager::Update()
{
	scurrentScene_->Update();

	//切り替えアニメーション開始
	if (sisSetNext_) {
		animeTimer_.AddTime(1);
		//シーン遷移
		if (animeTimer_.GetIsEnd()) {
			SceneChange();
		}
	}
}

void SceneManager::Draw()
{
	//ゲーム描画
	scurrentScene_->Draw();
}

void SceneManager::SceneChange()
{
	if (ssceneName_ == SceneName::Title) {
		Transition<TitleScene>();
	}
	else if (ssceneName_ == SceneName::Game) {
		Transition<GameScene>();
	}
	else if (ssceneName_ == SceneName::GameOver) {
		//Transition<GameScene>();
	}

	animeTimer_.Reset();
	sisSetNext_ = false;
}


