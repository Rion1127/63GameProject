#include "SceneManager.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"

#include "GameScene.h"
#include "ParticleScene.h"
#include "DebugScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"

std::unique_ptr<EmptyScene> SceneManager::scurrentScene_ = nullptr;
SceneName SceneManager::ssceneName_;
bool SceneManager::sisSetNext_ = false;
Timer SceneManager::animeTimer_;
Sprite SceneManager::whiteSprite_;

void SceneManager::Ini()
{
	//�r���h�J�n���̃V�[��
	Transition<TitleScene>();


	animeTimer_.SetLimitTime(100);
	whiteSprite_.Ini();
	whiteSprite_.SetColor(Color(220, 220, 220, 0));
	whiteSprite_.SetAnchor({0.f,0.f});
	whiteSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
}

void SceneManager::Update()
{
	scurrentScene_->Update();

	//�؂�ւ��A�j���[�V�����J�n
	if (sisSetNext_)
	{
		animeTimer_.AddTime(1);
		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();
		//�V�[���J��
		if (animeTimer_.GetIsEnd())
		{
			SceneChange();
		}
	}
	else
	{
		if (animeTimer_.GetTimer() > 0)
		{
			animeTimer_.AddTime(-3);
		}

		Color color = whiteSprite_.GetColor();
		color.a = 255.f * animeTimer_.GetTimeRate();
		whiteSprite_.SetColor(color);
		whiteSprite_.Update();

	}

}

void SceneManager::Draw()
{
	//�Q�[���`��
	scurrentScene_->Draw();

	PipelineManager::PreDraw("Sprite",TRIANGLELIST);
	whiteSprite_.Draw();
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

		//�Q�[���V�[����
		if (sceneName == SceneName::Game)
		{
			animeTimer_.SetLimitTime(100);
		}
		//�Q�[���I�[�o�[�V�[����
		else if (sceneName == SceneName::GameOver)
		{
			animeTimer_.SetLimitTime(120);
		}
	}
}
