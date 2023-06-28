#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"

GameScene::~GameScene()
{
	
}

void GameScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	Model::Ini();
	
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLightGroup(lightManager_->GetLightGroup().get());

	
	floor_ = std::move(std::make_unique<Object3d>());
	floor_->SetModel(Model::CreateOBJ_uniptr("cube", true));

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());
}

void GameScene::Update()
{
	gameCamera_.Update();
	Camera::scurrent_.eye_ = gameCamera_.GetCamera()->eye_;
	Camera::scurrent_.up_ = gameCamera_.GetCamera()->up_;
	Camera::scurrent_.target_ = gameCamera_.GetCamera()->target_;
	Camera::scurrent_.Update();

	//カメラ更新
	debugCamera_.Update();

	static float rotY = 0;
	rotY += 0.01f;
	
	
	floor_->SetPos({ 0,-1.f,0 });
	floor_->SetRot({ 0,0,0 });
	floor_->SetScale({ 30,1,30 });
	floor_->Update();

	player_->Update();

	lightManager_->DebugUpdate();
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
	PipelineManager::PreDraw("Toon", TRIANGLELIST);
	//skyDome_->Draw();
	
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	

	////////////
	//スプライト//
	////////////
	
}
