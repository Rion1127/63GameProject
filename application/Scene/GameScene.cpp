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

	skyDome_ = std::move(std::make_unique<Object3d>());
	skyDome_->SetModel(Model::CreateOBJ_uniptr("uvSphere", true));

	sphere_ = std::move(std::make_unique<Object3d>());
	sphere_->SetModel(Model::CreateOBJ_uniptr("uvSphere", true));

	cube_ = std::move(std::make_unique<Object3d>());
	cube_->SetModel(Model::CreateOBJ_uniptr("cube", true));

	player_ = std::move(std::make_unique<Player>());
	
}

void GameScene::Update()
{
	Camera::scurrent_.eye_ = debugCamera_.GetCamera()->eye_;
	Camera::scurrent_.up_ = debugCamera_.GetCamera()->up_;
	Camera::scurrent_.target_ = debugCamera_.GetCamera()->target_;
	Camera::scurrent_.Update();

	//カメラ更新
	debugCamera_.Update();

	static float rotY = 0;
	rotY += 0.01f;
	
	skyDome_->SetPos({ -2,0,0 });
	skyDome_->SetRot({ 0,rotY,0 });
	sphere_->SetPos({ 2,0,0 });
	sphere_->SetRot({ 0,rotY,0 });
	cube_->SetPos({ 2,0,0 });
	cube_->SetRot({ 0,rotY,0 });

	skyDome_->Update();
	sphere_->Update();
	cube_->Update();

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
	cube_->Draw();
	player_->Draw();
	PipelineManager::PreDraw("Toon", TRIANGLELIST);
	skyDome_->Draw();
	
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	

	////////////
	//スプライト//
	////////////
	
}
