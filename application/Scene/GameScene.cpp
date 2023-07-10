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
	

	
	floor_ = std::move(std::make_unique<Floor>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());

	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(floor_.get());
	colManager_->SetEnemys(enemyManager_.get());
	IAttack::SetPlayerInfo(player_->GetPlayerInfo());

	////const wchar_t* modelFile = L"Resources/Alicia/FBX/Alicia_solid_Unity.FBX";
	//const wchar_t* modelFile = L"application/Resources/boneTest/boneTest.fbx";
	////  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	////  L"Resources/FBX/untitled.glb"
	//std::vector<Mesh> meshes;
	//ImportSettings importSetting = {
	//	modelFile,
	//	meshes,
	//	false,
	//	true
	//};
	//
	//assimpObj_.SetModel(&testModel_);
	//testModel_ = AssimpLoader::GetInstance()->Load("application/Resources/boneTest/TestCube.fbx");
	//assimpObj_.SetModel(AssimpLoader::GetInstance()->Load("application/Resources/boneTest/boneTest.fbx",nullptr));
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

	assimpObj_.SetPos({ -2,2,0 });
	assimpObj_.Update();
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
	assimpObj_.Draw();
	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();

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
