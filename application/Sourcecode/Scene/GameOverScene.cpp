#include "GameOverScene.h"

#include "TitleScene.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "SoundVolume.h"

/**
 * @file GameOverScene.cpp
 * @brief ゲームオーバーシーンクラス
 */

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Ini()
{
	sound_ = SoundManager::GetInstance();

	titleSprite_ = std::move(std::make_unique<Sprite>());
	continueSprite_ = std::move(std::make_unique<Sprite>());
	backSprite_ = std::move(std::make_unique<Sprite>());

	playerObj_ = std::move(std::make_unique<Object3d>());
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true, false));
	playerObj_->SetShadowOffsetPos(Vector3(0,-1,0));
	playerObj_->WT_.SetRotType(RotType::Quaternion);

	titleSprite_->Ini();
	continueSprite_->Ini();
	backSprite_->Ini();

	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	continueSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("Continue"));
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("TitleTex"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.4f
	};

	continueSprite_->SetPos(pos);

	pos.y = WinAPI::GetWindowSize().y / 1.2f;
	titleSprite_->SetPos(pos);

	backSprite_->SetAnchor({ 0,0 });
	backSprite_->SetColor(Color(0, 0, 0, 255));

	selectType_ = SelectType::Continue;

	Camera::scurrent_ = &camera_;

	floatTimer_.SetLimitTime(160);
	floatTimer_.SetIsLoop(true);
}

void GameOverScene::Update()
{
	
	//メニュー選択
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetTriggerButtons(PAD::INPUT_UP))
	{
		SoundManager::Play("SelectSE", false, SoundVolume::GetValumeSE());
		bool type = (selectType_ == SelectType::Continue);
		//現在選択しているものがコンティニューだったら
		selectType_ = (type) ? SelectType::Title : SelectType::Continue;
	}

	Color selectColor = { 200,50,50,255 };
	if (selectType_ == SelectType::Continue)
	{
		continueSprite_->SetColor(selectColor);
		titleSprite_->SetColor(Color(255, 255, 255, 255));

	}
	else if (selectType_ == SelectType::Title)
	{
		continueSprite_->SetColor(Color(255, 255, 255, 255));
		titleSprite_->SetColor(selectColor);
	}
	//決定
	if (Controller::GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE))
	{
		if (SceneManager::GetIsSetNext() == false) {
			SoundManager::Play("EnterSE", false, SoundVolume::GetValumeSE());
		}
		if (selectType_ == SelectType::Continue)
		{
			SceneManager::SetChangeStart(SceneName::Game);
		}
		else if (selectType_ == SelectType::Title)
		{
			SceneManager::SetChangeStart(SceneName::Title);
		}
	}

	Camera::scurrent_->eye_ = Vector3(0, 5, -10);
	CameraUpdate();

	auto resultQ = Quaternion(0, 1, 0, 1)* Quaternion(-1, 0, 0, 1);
	playerObj_->WT_.quaternion_ = resultQ;


	float posY = UpAndDown(floatTimer_.GetLimitTimer(),0.3f, floatTimer_.GetTimer());
	floatTimer_.AddTime(1);

	playerObj_->SetPos(Vector3(0,2 + posY,0));
	playerObj_->Update();

	backSprite_->Update();
	continueSprite_->Update();
	titleSprite_->Update();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backSprite_->Draw();
	continueSprite_->Draw();
	titleSprite_->Draw();

	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	playerObj_->Draw();
	PipelineManager::PreDraw("Toon", TRIANGLELIST);

	PipelineManager::PreDraw("assimp", TRIANGLELIST);


	PipelineManager::PreDraw("Sprite", TRIANGLELIST);


	PipelineManager::PreDraw("Particle", POINTLIST);

}

void GameOverScene::DrawRenderTexture()
{
}

void GameOverScene::DrawPostEffect()
{
}

void GameOverScene::CameraUpdate()
{

	Camera::scurrent_->Update(CameraMode::LookAT);
}
