#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"

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
	//3D�I�u�W�F�N�g//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	floor_->Draw();
	player_->Draw();
	enemyManager_->Draw();
	
	
	PipelineManager::PreDraw("Toon", TRIANGLELIST);
	
	
	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//�X�v���C�g//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void GameScene::CameraUpdate()
{
	static bool isDebug = true;
	if (Key::TriggerKey(DIK_Q)) {
		isDebug = isDebug == false ? true : false;
	}
	if (isDebug)
	{
		gameCamera_.Update(CameraMode::LookTo);
		Camera::scurrent_ = gameCamera_.GetCamera();
	}
	else
	{
		//�J�����X�V
		debugCamera_.Update();
		Camera::scurrent_ = debugCamera_.GetCamera();
	}
	Camera::scurrent_->Update(CameraMode::LookTo);

	
}
