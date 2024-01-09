#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "EnemyLoader.h"
#include "Framework.h"

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
	pauseMenu_ = std::make_unique<PauseMenu>();

	stage_ = std::move(std::make_unique<Stage>());

	player_ = std::move(std::make_unique<Player>());
	gameCamera_.SetPlayer(player_.get());
	IEnemy::SetPlayer(player_.get());
	colManager_->SetPlayer(player_.get());
	colManager_->SetFloor(stage_.get());
	colManager_->SetEnemys(enemyManager_.get());
	colManager_->SetCamera(&gameCamera_);
	AttackManager::SetPlayer(player_.get());
	enemyManager_->SetPlayer(player_.get());

	JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");

	std::unique_ptr<AssimpModel> model_;

	std::string fileName = "application/Resources/Object/boneTest.fbx";

	model_ = AssimpLoader::GetInstance()->Load(fileName);

	testObj_ = std::move(std::make_unique<Object3d>());
	testObj_->SetModel(Model::CreateOBJ_uniptr("wave", false, false));
}

void DebugScene::Update()
{
#ifdef _DEBUG
	//ステージオブジェホットリロード
	if (Key::TriggerKey(DIK_R))
	{
		ParticleManager::GetInstance()->AllClear();
		JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
		JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
	}
	//敵情報ホットリロード
	if (Key::TriggerKey(DIK_E))
	{
		std::string path = "application/Resources/EnemyPOP/";
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "DebugEnemy.csv", "Debug");
		EnemyLoader::GetInstance()->LoadEnemyPopFile(path + "HadesCup.csv", "HadesCup");
	}
	if (Key::TriggerKey(DIK_G))
	{
		std::shared_ptr<OneceEmitter> hitEmitter_ = std::make_shared<OneceEmitter>();
		hitEmitter_->pos = {0,0.01f,0};
		ParticleManager::GetInstance()->
			AddParticle("groundStump", hitEmitter_);
	}
#endif // _DEBUG

	if (pauseMenu_->GetIsPause() == false) {
		CameraUpdate();

		//当たり判定前更新
		stage_->Update();
		player_->PreColUpdate();
		//デバッグ用
		if (Framework::isImguiDisplay_) enemyManager_->DebugUpdate();
		if (Framework::isImguiDisplay_)LoadEnemyImGui();
		enemyManager_->PreColUpdate();
		//当たり判定
		colManager_->Update();
		//当たり判定後更新
		enemyManager_->PostUpdate();
		player_->PostUpdate();
		//その他
		lightManager_->Update();
		ParticleManager::GetInstance()->Update();
		//プレイヤーが死んだらシーン変更
		if (player_->GetIsAlive() == false)
		{
			SceneManager::SetChangeStart(SceneName::GameOver);
		}
	}
	pauseMenu_->Update();

	testObj_->WT_.position_.y = 2;
	testObj_->WT_.rotation_.x = Radian(90);
	testObj_->Update();
}

void DebugScene::Draw()
{

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	stage_->Draw();
	enemyManager_->Draw();
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteBackDraw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	player_->Draw();
	PipelineManager::PreDraw("Wave", TRIANGLELIST);
	testObj_->Draw();

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	enemyManager_->SpriteDraw();
	player_->DrawSprite();
	pauseMenu_->Draw();


	if (Framework::isImguiDisplay_) {
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

		colManager_->DrawImGui();
		player_->DrawImGui();
		gameCamera_.DrawImGui();
	}
}

void DebugScene::DrawRenderTexture()
{
	


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
