#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "EnemyLoader.h"

#include "JsonLoader.h"
#include <imgui.h>
#include "ParticleTest.h"
#include "ParticleEnemyDead.h"
#include "ParticleFire.h"

#include <time.h>

DebugScene::~DebugScene()
{

}

void DebugScene::Ini()
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

	JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	//EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), "Debug", 1);

	uint32_t maxRoundNum = (uint32_t)EnemyLoader::GetInstance()->GetEnemyData("Debug").size();
	colosseumSystem_->SetMaxRoundNum(maxRoundNum);

	std::unique_ptr<AssimpModel> model_;

	std::string fileName = "application/Resources/Object/boneTest.fbx";

	model_ = AssimpLoader::GetInstance()->Load(fileName, model_.get());

	//obj_.SetModel(std::move(model_));
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
	if (pauseMenu_->GetIsPause() == false) {
		//colosseumSystem_->Update();

		CameraUpdate();
		//当たり判定前更新
		stage_->Update();
		player_->PreUpdate();
		enemyManager_->PreUpdate();

#ifdef _DEBUG
		LoadEnemyImGui();
#endif // _DEBUG
		//当たり判定
		colManager_->Update();
		//当たり判定後更新
		enemyManager_->PostUpdate();
		player_->PostUpdate();
		//その他
		lightManager_->DebugUpdate();
		ParticleManager::GetInstance()->Update();
		//UI更新
		operationUI_->Update();
		//プレイヤーが死んだらシーン変更
		if (player_->GetIsAlive() == false)
		{
			SceneManager::SetChangeStart(SceneName::GameOver);
		}

		if (Key::TriggerKey(DIK_P))
		{
			std::shared_ptr<OneceEmitter> debugEmitter1 = std::make_shared<OneceEmitter>();
			debugEmitter1->particle = std::make_unique<ParticleTest>();
			debugEmitter1->addNum = 10;
			debugEmitter1->time = 60000;
			debugEmitter1->pos = { -3,3,0 };
			debugEmitter1->addVec = { 1,1,1 };
			debugEmitter1->scale = 1.0f;

			ParticleManager::GetInstance()->
				AddParticle("WallHit", debugEmitter1);

			std::shared_ptr<OneceEmitter> debugEmitter2 = std::make_shared<OneceEmitter>();
			debugEmitter2->particle = std::make_unique<ParticleEnemyDead>();
			debugEmitter2->addNum = 64;
			debugEmitter2->time = 60;
			debugEmitter2->pos = { 3,3,0 };
			debugEmitter2->addVec = { 1,1,1 };
			debugEmitter2->scale = 1.0f;

			ParticleManager::GetInstance()->
				AddParticle("EnemyDead", debugEmitter2);
		}

		if (Key::TriggerKey(DIK_P))
		{
			Timer timer;
			timer.SetLimitTime(1);
			fireEmitter_ = std::make_shared<ContinuousEmitter>();
			fireEmitter_->particle = std::make_unique<ParticleFire>();
			fireEmitter_->addVec = { 0.2f,0.2f, 0.2f, };
			fireEmitter_->addNum = 2;
			fireEmitter_->isActive = true;
			fireEmitter_->popCoolTime_ = timer;
			fireEmitter_->time = 120;
			fireEmitter_->pos = { 0,3,0 };
			fireEmitter_->scale = 1.f;
			ParticleManager::GetInstance()->AddParticle("fireBall", fireEmitter_);
		}
		if (fireEmitter_ != nullptr)
		{
			fireEmitter_->popCoolTime_.AddTime(1);
			if (fireEmitter_->popCoolTime_.GetIsEnd()) {
				fireEmitter_->particle->Add();

				fireEmitter_->popCoolTime_.Reset();
			}
			fireEmitter_->particle->Update();
		}

	}
	pauseMenu_->Update();

	//obj_.SetPos({ 0, 2, 0 });
	//obj_.SetScale({ 1,1,1 });

	//obj_.Update();
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
	//obj_.Draw();
	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();
	player_->DrawSprite();
	operationUI_->Draw();
	//colosseumSystem_->DrawSprite();
	pauseMenu_->Draw();

	
}

void DebugScene::DrawPostEffect()
{
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

void DebugScene::LoadEnemyImGui()
{
	std::string fileName_;
	ImGui::Begin("EnemyRound");

	//データプールのハッシュ値をfileNameに代入
	std::vector<std::string> fileName;
	auto datasize = EnemyLoader::GetInstance()->GetAllData();
	for (auto itr = datasize.begin(); itr != datasize.end(); ++itr)
	{
		fileName.push_back(itr->first);
	}
	//プルダウンメニューで読み込んだファイルを選択できるようにする
	static std::string s_currentItem;
	if (ImGui::BeginCombo("fileName", s_currentItem.c_str()))
	{
		for (int i = 0; i < fileName.size() - 1; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (s_currentItem == fileName[i]);
			if (ImGui::Selectable(fileName[i].c_str(), is_selected))
			{
				s_currentItem = fileName[i].c_str();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	//読み込んだファイルのラウンド数を入力
	size_t RoundMax = EnemyLoader::GetInstance()->GetEnemyData(s_currentItem).size();
	static int roundNum = 0;
	ImGui::DragInt("Round", &roundNum, 1.f, 1, (int)RoundMax);
	//上記の内容でファイルをリロードする
	if (ImGui::Button("RoundLoad"))
	{
		EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), s_currentItem, roundNum);
	}
	ImGui::End();
}
