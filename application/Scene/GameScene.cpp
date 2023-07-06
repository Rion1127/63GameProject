#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"

GameScene::~GameScene()
{
	
}

void GameScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();
	
	lightManager_ = std::make_shared<LightManager>();
	colManager_ = std::make_unique<CollisionManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLightGroup(lightManager_->GetLightGroup().get());

	
	floor_ = std::move(std::make_unique<Floor>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());

	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(floor_.get());
	colManager_->SetEnemys(enemyManager_.get());
	IAttack::SetPlayerInfo(player_->GetPlayerInfo());
}

void GameScene::Update()
{
	CameraUpdate();
	
	floor_->Update();
	player_->PreUpdate();
	enemyManager_->PreUpdate();

	colManager_->Update();

	enemyManager_->PostUpdate();
	player_->PostUpdate();

	lightManager_->DebugUpdate();
	ParticleManager::GetInstance()->Update();
}

void GameScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	
	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	floor_->Draw();
	player_->Draw();
	enemyManager_->Draw();
	PipelineManager::PreDraw("Toon", TRIANGLELIST);
	//skyDome_->Draw();
	
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	

	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void GameScene::CameraUpdate()
{
	if (Key::PushKey(DIK_LCONTROL) == false)
	{
		gameCamera_.Update();
		Camera::scurrent_.eye_ = gameCamera_.GetCamera()->eye_;
		Camera::scurrent_.up_ = gameCamera_.GetCamera()->up_;
		Camera::scurrent_.target_ = gameCamera_.GetCamera()->target_;
	}
	else
	{
		//カメラ更新
		debugCamera_.Update();
		Camera::scurrent_.eye_ = debugCamera_.GetCamera()->eye_;
		Camera::scurrent_.up_ = debugCamera_.GetCamera()->up_;
		Camera::scurrent_.target_ = debugCamera_.GetCamera()->target_;
	}
	Camera::scurrent_.Update();

	
}
