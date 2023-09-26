#include "GameOverScene.h"

#include "TitleScene.h"
#include "SceneManager.h"
#include "GameScene.h"
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

	playerObj_->SetModel(Model::CreateOBJ_uniptr("Player"));

	selectType_ = SelectType::Continue;
}

void GameOverScene::Update()
{
	CameraUpdate();

	//メニュー選択
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetTriggerButtons(PAD::INPUT_UP))
	{
		SoundManager::Play("SelectSE");
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
			SoundManager::Play("EnterSE", false, 1.5f);
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

	PipelineManager::PreDraw("Toon", TRIANGLELIST);

	PipelineManager::PreDraw("assimp", TRIANGLELIST);


	PipelineManager::PreDraw("Sprite", TRIANGLELIST);


	PipelineManager::PreDraw("Particle", POINTLIST);

}

void GameOverScene::DrawPostEffect()
{
}

void GameOverScene::CameraUpdate()
{

	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
