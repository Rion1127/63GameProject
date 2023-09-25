#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "EnemyLoader.h"
#include "JsonLoader.h"

GameScene::~GameScene()
{

}

void GameScene::Ini()
{
	sound_ = SoundManager::GetInstance();

	lightManager_ = std::make_shared<LightManager>();
	colManager_ = std::make_unique<CollisionManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	operationUI_ = std::make_unique<UIOperation>();
	colosseumSystem_ = std::make_unique<ColosseumSystem>();
	pauseMenu_ = std::make_unique<PauseMenu>();

	stage_ = std::move(std::make_unique<Stage>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());

	IEnemy::SetPlayer(player_.get());
	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(stage_.get());
	colManager_->SetEnemys(enemyManager_.get());
	AttackManager::SetPlayer(player_.get());
	enemyManager_->SetPlayer(player_.get());
	colosseumSystem_->SetPlayer(player_.get());
	colosseumSystem_->SetEnemy(enemyManager_.get());

	JsonLoader::GetInstance()->LoadFile("stage.json","Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(),"Stage");

	cupName_ = "HadesCup";

	EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), cupName_, 1);

	uint32_t maxRoundNum = (uint32_t)EnemyLoader::GetInstance()->GetEnemyData(cupName_).size();
	colosseumSystem_->SetMaxRoundNum(maxRoundNum);

	lightColor_ = { 1,1,1 };
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (Key::TriggerKey(DIK_R))
	{
		JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
		JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	}
#endif // _DEBUG
	if (GetIsGameStop()) {
		CameraUpdate();

		stage_->Update();
		player_->PreUpdate();
		enemyManager_->PreUpdate();

		colManager_->Update();

		enemyManager_->PostUpdate();
		player_->PostUpdate();

		
		lightManager_->GetLightGroup()->SetDirLightColor(0, lightColor_);

		lightManager_->Update();
		ParticleManager::GetInstance()->Update();

		operationUI_->Update();

		if (player_->GetIsAlive() == false)
		{
			SceneManager::SetChangeStart(SceneName::GameOver);
		}

		if (Key::TriggerKey(DIK_P)) {

		}

		if (colosseumSystem_->GetIsReset())
		{
			player_->Reset();
			enemyManager_->Reset();
			gameCamera_.Reset();
			uint32_t nextRound = colosseumSystem_->GetRoundNum();
			EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), cupName_, nextRound);
			colosseumSystem_->SetIsReset(false);

			uint32_t maxRoundNum = (uint32_t)EnemyLoader::GetInstance()->GetEnemyData(cupName_).size();
			float rate = 1.f - ((float)nextRound / (float)maxRoundNum);
			float colGB = 1.f * rate;
			lightColor_ = { 1.f,colGB,colGB };
		}
	}
	colosseumSystem_->Update();
	pauseMenu_->Update();

}

void GameScene::Draw()
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
	colosseumSystem_->DrawSprite();
	pauseMenu_->Draw();

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
}

void GameScene::DrawPostEffect()
{
}

void GameScene::CameraUpdate()
{
	static bool isDebug = true;
	if (Key::TriggerKey(DIK_Q)) {
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

bool GameScene::GetIsGameStop()
{
	if (pauseMenu_->GetIsPause() == false && 
		colosseumSystem_->GetIsClear() == false)
	{
		return true;
	}
	return false;
}
