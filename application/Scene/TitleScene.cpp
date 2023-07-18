#include "TitleScene.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	lightManager_ = std::make_shared<LightManager>();
	colManager_ = std::make_unique<CollisionManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	Model::SetLight(lightManager_->GetLightGroup());
}

void TitleScene::Update()
{
	CameraUpdate();
	lightManager_->DebugUpdate();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);

	PipelineManager::PreDraw("Toon", TRIANGLELIST);

	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	

	PipelineManager::PreDraw("Particle", POINTLIST);
	
}

void TitleScene::CameraUpdate()
{
	
	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
