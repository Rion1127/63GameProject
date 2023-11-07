#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "EnemyLoader.h"

#include "JsonLoader.h"
#include <imgui.h>
#include "GameSpeed.h"
#include "ConfigMenu.h"

#include <time.h>

/**
 * @file DebugScene.cpp
 * @brief デバッグ用のシーンクラス
 */

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
	gameCamera_.SetColosseumSystem(colosseumSystem_.get());
	IEnemy::SetPlayer(player_.get());
	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(stage_.get());
	colManager_->SetEnemys(enemyManager_.get());
	colManager_->SetCamera(&gameCamera_);
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

	model_ = AssimpLoader::GetInstance()->Load(fileName);

	//obj_.SetModel(std::move(model_));
}

void DebugScene::Update()
{
#ifdef _DEBUG
	if (Key::TriggerKey(DIK_R))
	{
		ParticleManager::GetInstance()->AllClear();
		JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
		JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	}
	if (Key::TriggerKey(DIK_E))
	{
		std::string path = "application/Resources/EnemyPOP/";
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "DebugEnemy.csv", "Debug");
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "HadesCup.csv", "HadesCup");
	}
#endif // _DEBUG

	if (pauseMenu_->GetIsPause() == false) {
		//colosseumSystem_->Update();
		CameraUpdate();

		//当たり判定前更新
		stage_->Update();
		player_->PreUpdate();
		enemyManager_->DebugUpdate();
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

	}
	pauseMenu_->Update();
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

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();

	ImGui::Begin("GameSpeed");

	static float gamespeed = 1;
	ImGui::DragFloat("GameSpeed", &gamespeed, 0.1f);
	GameSpeed::SetGameSpeed(gamespeed);
	static float playerspeed = 1;
	ImGui::DragFloat("PlayerSpeed", &playerspeed, 0.1f);
	GameSpeed::SetPlayerSpeed(playerspeed);
	static float enemyspeed = 1;
	ImGui::DragFloat("EnemySpeed", &enemyspeed, 0.1f);
	GameSpeed::SetEnemySpeed(enemyspeed);

	ImGui::End();

	ConfigMenu::GetInstance()->DrawImGui();
	colManager_->DrawImGui();
	player_->DrawImGui();
	gameCamera_.DrawImGui();
}

void DebugScene::DrawRenderTexture()
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
	static int roundNum = 1;
	std::string fileName_;
	ImGui::Begin("EnemyRound");

	//データプールのハッシュ値をfileNameに代入
	std::vector<std::string> fileName;
	auto datasize = EnemyLoader::GetInstance()->GetAllData();
	for (auto itr = datasize.begin(); itr != datasize.end(); ++itr)
	{
		if (itr->first == "")continue;
		fileName.push_back(itr->first);
	}
	//プルダウンメニューで読み込んだファイルを選択できるようにする
	static std::string s_currentItem;
	if (ImGui::BeginCombo("fileName", s_currentItem.c_str()))
	{
		for (int i = 0; i < fileName.size(); ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (s_currentItem == fileName[i]);
			if (ImGui::Selectable(fileName[i].c_str(), is_selected))
			{
				s_currentItem = fileName[i].c_str();
				roundNum = 1;
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

	ImGui::DragInt("Round", &roundNum, 1.f, 1, (int)RoundMax);
	//上記の内容でファイルをリロードする
	if (ImGui::Button("RoundLoad"))
	{
		EnemyLoader::GetInstance()->SetEnemy(enemyManager_->GetEnemy(), s_currentItem, roundNum);
	}
	if (ImGui::Button("HotReLoad"))
	{
		std::string path = "application/Resources/EnemyPOP/";
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "DebugEnemy.csv", "Debug");
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "HadesCup.csv", "HadesCup");
	}
	ImGui::End();
}
