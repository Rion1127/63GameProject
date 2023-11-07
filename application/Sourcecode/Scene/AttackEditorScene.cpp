#include "AttackEditorScene.h"
#include "JsonLoader.h"

/**
 * @file AttackEditorScene.cpp
 * @brief アタックエディタ用のシーンクラス
 */

AttackEditorScene::~AttackEditorScene()
{
}

void AttackEditorScene::Ini()
{
	stage_ = std::move(std::make_unique<Stage>());
	JsonLoader::GetInstance()->LoadFile("stage.json", "Stage");
	JsonLoader::GetInstance()->SetObjects(stage_->GetObjects(), "Stage");
}

void AttackEditorScene::Update()
{
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();

	attackEditor_.Update();

	//当たり判定前更新
	stage_->Update();
}

void AttackEditorScene::Draw()
{
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	stage_->Draw();
	attackEditor_.DrawImGui();
	attackEditor_.Draw();
}

void AttackEditorScene::DrawRenderTexture()
{
	attackEditor_.DrawRenderTexture();
}

void AttackEditorScene::CameraUpdate()
{
}

void AttackEditorScene::LoadEnemyImGui()
{
}
