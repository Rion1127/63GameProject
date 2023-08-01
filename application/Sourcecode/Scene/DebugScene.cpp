#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"

#include "JsonLoader.h"

DebugScene::~DebugScene()
{

}

void DebugScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	lightManager_ = std::make_shared<LightManager>();
	colManager_ = std::make_unique<CollisionManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	operationUI_ = std::make_unique<UIOperation>();

	stage_ = std::move(std::make_unique<Stage>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());

	IEnemy::SetPlayer(player_.get());
	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(stage_.get());
	colManager_->SetEnemys(enemyManager_.get());
	AttackManager::SetPlayer(player_.get());
	enemyManager_->SetPlayer(player_.get());

	JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
}

void DebugScene::Update()
{
#ifdef _DEBUG
	if (Key::TriggerKey(DIK_R))
	{
		JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
		JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	}
#endif // _DEBUG

	CameraUpdate();

	stage_->Update();
	player_->PreUpdate();
	enemyManager_->PreUpdate();

	colManager_->Update();

	enemyManager_->PostUpdate();
	player_->PostUpdate();

	lightManager_->DebugUpdate();
	ParticleManager::GetInstance()->Update();

	operationUI_->Update();

	if (player_->GetIsAlive() == false)
	{
		SceneManager::SetChangeStart(SceneName::GameOver);
	}

	if (Key::TriggerKey(DIK_P))
	{
		ParticleManager::GetInstance()->
			AddParticle("EnemyDead", 9, 80, { 0,2,0 }, { 0.5f,0.5f, 0.5f }, 2.f);
	}

}

void DebugScene::Draw()
{

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	stage_->Draw();
	player_->Draw();
	enemyManager_->Draw();

	PipelineManager::PreDraw("Toon", TRIANGLELIST);


	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();
	player_->DrawSprite();
	operationUI_->Draw();

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void DebugScene::CameraUpdate()
{
	static bool isDebug = true;
	if (Key::TriggerKey(DIK_Q))
	{
		isDebug = isDebug == false ? true : false;
	}
	if (isDebug)
	{
		gameCamera_.Update(CameraMode::LookAT);
		Camera::scurrent_ = gameCamera_.GetCamera();
	}
	else
	{
		//カメラ更新
		debugCamera_.Update();
		Camera::scurrent_ = debugCamera_.GetCamera();
	}
	Camera::scurrent_->Update(CameraMode::LookAT);
}
