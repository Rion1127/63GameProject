#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"
#include "DebugScene.h"


void SceneManager::Ini()
{
	//�r���h�J�n���̃V�[��
	Transition<GameScene>();
	

	
}

void SceneManager::Update()
{
	scurrentScene_->Update();

	
}

void SceneManager::Draw()
{
	//�Q�[���`��
	scurrentScene_->Draw();
}

std::unique_ptr<EmptyScene> SceneManager::scurrentScene_ = nullptr;

