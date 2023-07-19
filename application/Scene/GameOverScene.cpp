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
	titleSprite_ = std::move(std::make_unique<Sprite>());
	retrySprite_ = std::move(std::make_unique<Sprite>());
	backSprite_ = std::move(std::make_unique<Sprite>());

	pressASprite_->Ini();
	titleSprite_->Ini();
	retrySprite_->Ini();
	backSprite_->Ini();

	pressASprite_->SetTexture(TextureManager::GetInstance()->GetTexture("PressA"));
	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.2f
	};
	pressASprite_->SetPos(pos);

	backSprite_->SetAnchor({ 0,0 });
	backSprite_->SetColor(Color(0, 0, 0, 255));
}

void GameOverScene::Update()
{
	CameraUpdate();

	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
		if (SceneManager::GetIsSetNext() == false) {
			SoundManager::Play("EnterSE", false, 1.5f);
		}
		SceneManager::SetChangeStart(SceneName::Game);
	}
	backSprite_->Update();
	pressASprite_->Update();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backSprite_->Draw();
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
