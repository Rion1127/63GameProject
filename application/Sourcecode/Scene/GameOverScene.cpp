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


	SpriteInit();

	playerObj_ = std::move(std::make_unique<Object3d>());
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true, false));
	playerObj_->SetShadowOffsetPos(Vector3(0,-1,0));
	playerObj_->WT_.SetRotType(RotType::Quaternion);

	

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

	Color selectColor = { 230,50,50,255 };
	Color unSelectColor = { 0,35,255,255 };
	if (selectType_ == SelectType::Continue)
	{
		continueBackSprite_->SetColor(selectColor);
		continueBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
		titleBackSprite_->SetColor(unSelectColor);
		titleBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));

	}
	else if (selectType_ == SelectType::Title)
	{
		continueBackSprite_->SetColor(unSelectColor);
		continueBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		titleBackSprite_->SetColor(selectColor);
		titleBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
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

	Camera::scurrent_->eye_ = Vector3(0, 1, -15);
	Camera::scurrent_->target_ = Vector3(0,1,0);
	CameraUpdate();

	auto resultQ = Quaternion(0, 1, 0, 1)* Quaternion(-1, 0, 0, 1);
	playerObj_->WT_.quaternion_ = resultQ;


	float posY = UpAndDown(floatTimer_.GetLimitTimer(),0.3f, floatTimer_.GetTimer());
	floatTimer_.AddTime(1);

	playerObj_->SetPos(Vector3(0,2 + posY,0));
	playerObj_->Update();

	backSprite_->Update();
	continueSprite_->Update();
	continueBackSprite_->Update();
	titleSprite_->Update();
	titleBackSprite_->Update();
}

void GameOverScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backSprite_->Draw();

	continueBackSprite_->Draw();
	continueSprite_->Draw();

	titleBackSprite_->Draw();
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
	Camera::scurrent_ = &camera_;
	Camera::scurrent_->Update(CameraMode::LookAT);
}

void GameOverScene::SpriteInit()
{
	titleSprite_ = std::move(std::make_unique<Sprite>());
	titleBackSprite_ = std::move(std::make_unique<Sprite>());
	continueSprite_ = std::move(std::make_unique<Sprite>());
	continueBackSprite_ = std::move(std::make_unique<Sprite>());
	backSprite_ = std::move(std::make_unique<Sprite>());

	titleSprite_->Ini();
	titleBackSprite_->Ini();
	continueSprite_->Ini();
	continueBackSprite_->Ini();
	backSprite_->Ini();

	backSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("White1280x720"));
	continueSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectTex"));
	continueBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
	titleSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectTex"));
	titleBackSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));

	Vector2 texSize = {
			160.f,
			38
	};
	Vector2 texScale = {
			(1.f / 4.f) * 0.6f,
			0.6f
	};
	Vector2 frameScale = {
			0.6f,
			0.6f
	};
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.4f
	};

	continueSprite_->SetPos(pos);
	continueSprite_->SetTex_LeftTop(Vector2(0, 0));
	continueSprite_->SetTex_Size(texSize);
	continueSprite_->SetScale(texScale);
	continueBackSprite_->SetScale(frameScale);
	continueBackSprite_->SetPos(pos);

	pos.y = WinAPI::GetWindowSize().y / 1.2f;
	Vector2 titlespritePos = pos;
	titlespritePos.x += 10;
	titleSprite_->SetPos(titlespritePos);
	titleSprite_->SetTex_LeftTop(Vector2(160, 0));
	titleSprite_->SetTex_Size(texSize);
	titleSprite_->SetScale(texScale);
	titleBackSprite_->SetScale(frameScale);
	titleBackSprite_->SetPos(pos);

	backSprite_->SetAnchor({ 0,0 });
	backSprite_->SetColor(Color(0, 0, 0, 255));
}
