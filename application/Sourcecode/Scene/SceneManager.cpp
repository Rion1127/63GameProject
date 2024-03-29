#include "SceneManager.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"

#include "GameScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "AttackEditorScene.h"
#include "Framework.h"
#include "ColDebugScene.h"

/**
 * @file SceneManager.cpp
 * @brief シーンを管理するクラス
 */

std::unique_ptr<IScene> SceneManager::scurrentScene_ = nullptr;
SceneName SceneManager::ssceneName_;
bool SceneManager::sisSetNext_ = false;
Timer SceneManager::animeTimer_;
Sprite SceneManager::whiteSprite_;

void SceneManager::Ini()
{
#ifdef _DEBUG
	//ビルド開始時のシーン
	Transition<DebugScene>();
#else
	//ビルド開始時のシーン
	Transition<TitleScene>();
#endif // _DEBUG





	animeTimer_.SetLimitTime(100);
	whiteSprite_.Ini();
	whiteSprite_.SetColor(Color(220, 220, 220, 0));
	whiteSprite_.SetAnchor({ 0.f,0.f });
	whiteSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
}

void SceneManager::Update()
{
	scurrentScene_->Update();

#ifdef _DEBUG
	if (Key::PushKey(DIK_LSHIFT))
	{
		if (Key::TriggerKey(DIK_1))
		{
			Transition<TitleScene>();
			ssceneName_ = SceneName::Title;
		}
		if (Key::TriggerKey(DIK_2))
		{
			Transition<GameScene>();
			ssceneName_ = SceneName::Game;
		}
		if (Key::TriggerKey(DIK_3))
		{
			Transition<GameOverScene>();
			ssceneName_ = SceneName::GameOver;
		}
		if (Key::TriggerKey(DIK_0))
		{
			Transition<DebugScene>();
			ssceneName_ = SceneName::Debug;
		}
		if (Key::TriggerKey(DIK_9))
		{
			Transition<AttackEditorScene>();
			ssceneName_ = SceneName::AttackEditor;
		}
	}

	if (Framework::isImguiDisplay_) {
		ImGui::Begin("SceneChange");

		if (ImGui::Button("Title",ImVec2(50, 30))) {
			Transition<TitleScene>();
			ssceneName_ = SceneName::Title;
		}
		ImGui::SameLine();
		if (ImGui::Button("Game", ImVec2(50, 30))) {
			Transition<GameScene>();
			ssceneName_ = SceneName::Game;
		}
		if (ImGui::Button("Debug", ImVec2(50, 30))) {
			Transition<DebugScene>();
			ssceneName_ = SceneName::Debug;
		}
		ImGui::SameLine();
		if (ImGui::Button("Editor", ImVec2(50, 30))) {
			Transition<AttackEditorScene>();
			ssceneName_ = SceneName::AttackEditor;
		}
		if (ImGui::Button("ColDebug", ImVec2(50, 30))) {
			Transition<ColDebugScene>();
			ssceneName_ = SceneName::Debug;
		}

		ImGui::End();
	}

#endif // _DEBUG

	//切り替えアニメーション開始
	if (sisSetNext_)
	{
		animeTimer_.AddTime(1);
		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();
		//シーン遷移
		if (animeTimer_.GetIsEnd())
		{
			SceneChange();
		}
	}
	else
	{
		if (animeTimer_.GetTimer() > 0)
		{
			animeTimer_.AddTime(-5);
		}

		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();
	}
}

void SceneManager::Draw()
{
	//ゲーム描画
	scurrentScene_->Draw();

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	whiteSprite_.Draw();
}

void SceneManager::DrawPostEffect()
{
	scurrentScene_->DrawPostEffect();
}

void SceneManager::DrawRenderTexture()
{
	scurrentScene_->DrawRenderTexture();
}

void SceneManager::SceneChange()
{
	if (ssceneName_ == SceneName::Title)
	{
		Transition<TitleScene>();
	}
	else if (ssceneName_ == SceneName::Game)
	{
		Transition<GameScene>();
	}
	else if (ssceneName_ == SceneName::GameOver)
	{
		Transition<GameOverScene>();
	}

	sisSetNext_ = false;


}

void SceneManager::SetChangeStart(const SceneName sceneName) {
	if (sisSetNext_ == false)
	{

		ssceneName_ = sceneName;
		sisSetNext_ = true;

		//ゲームシーンへ
		if (sceneName == SceneName::Game)
		{
			animeTimer_.SetLimitTime(100);
		}
		//ゲームオーバーシーンへ
		else if (sceneName == SceneName::GameOver)
		{
			animeTimer_.SetLimitTime(120);
		}
	}
}
