#include "TitleScene.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "SoundVolume.h"

/**
 * @file TitleScene.cpp
 * @brief タイトルを管理するクラス
 */

TitleScene::~TitleScene()
{
}

void TitleScene::Ini()
{
	sound_ = SoundManager::GetInstance();

	pressASprite_ = std::move(std::make_unique<Sprite>());
	pressASprite_->Ini("pressA");
	pressASprite_->SetTexture(TextureManager::GetInstance()->GetTexture("PressA"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.2f
	};
	pressASprite_->SetPos(pos);

	titleSprite_ = std::move(std::make_unique<Sprite>());
	titleSprite_->Ini();
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Title"));
	Vector2 titlepos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.5f
	};
	titleSprite_->SetScale({ 0.5f,0.5f });
	titleSprite_->SetPos(titlepos);

	backSprite_ = std::move(std::make_unique<Sprite>());
	backSprite_->Ini();
	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	pos = { 0,0 };
	backSprite_->SetAnchor({ 0,0 });
	backSprite_->SetPos(pos);
	backSprite_->SetColor({ 150,150,150,255 });

	pressATimer_.SetLimitTime(120);
	pressATimer_.SetIsLoop(true);
}

void TitleScene::Update()
{
	CameraUpdate();

	if (Controller::GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
		if (SceneManager::GetIsSetNext() == false) {
			SoundManager::Play("EnterSE", false, SoundVolume::GetValumeSE());
		}
		SceneManager::SetChangeStart(SceneName::Game);
	}

	float pressASpriteAlpha = 122 + UpAndDown(pressATimer_.GetLimitTimer(), 122, pressATimer_.GetTimer());

	pressATimer_.AddTime(1);

	auto color = pressASprite_->GetColor();
	color.a = pressASpriteAlpha;

	pressASprite_->SetColor(color);

	pressASprite_->Update();
	titleSprite_->Update();
	backSprite_->Update();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backSprite_->Draw();
	pressASprite_->Draw();
	titleSprite_->Draw();

	PipelineManager::PreDraw("Object3D", TRIANGLELIST);

	PipelineManager::PreDraw("Toon", TRIANGLELIST);

	PipelineManager::PreDraw("assimp", TRIANGLELIST);


	PipelineManager::PreDraw("Sprite", TRIANGLELIST);


	PipelineManager::PreDraw("Particle", POINTLIST);

}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawPostEffect()
{
}

void TitleScene::CameraUpdate()
{

	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
