#include "GameOverScene.h"

#include "TitleScene.h"
#include "SceneManager.h"
#include "GameScene.h"
GameOverScene::~GameOverScene()
{
}

void GameOverScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	pressASprite_ = std::move(std::make_unique<Sprite>());
	pressASprite_->Ini();
	pressASprite_->SetTexture(TextureManager::GetInstance()->GetTexture("PressA"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.2f
	};
	pressASprite_->SetPos(pos);
}

void GameOverScene::Update()
{
	CameraUpdate();

	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
		SceneManager::SetChangeStart(SceneName::Game);
		SoundManager::Play("EnterSE", false, 1.5f);
	}

	pressASprite_->Update();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	pressASprite_->Draw();

	PipelineManager::PreDraw("Object3D", TRIANGLELIST);

	PipelineManager::PreDraw("Toon", TRIANGLELIST);

	PipelineManager::PreDraw("assimp", TRIANGLELIST);


	PipelineManager::PreDraw("Sprite", TRIANGLELIST);


	PipelineManager::PreDraw("Particle", POINTLIST);

}

void GameOverScene::CameraUpdate()
{

	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
