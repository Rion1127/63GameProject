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

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
}

void AttackEditorScene::Update()
{
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();

	attackEditor_.Update();

	lightManager_->Update();

	//当たり判定前更新
	stage_->Update();
}

void AttackEditorScene::Draw()
{
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	stage_->Draw();
	attackEditor_.DrawImGui();
	attackEditor_.Draw();

	ImGui::Begin("AttackEditorInfo");

	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Ctrl + mouseLeft :        MoveCameraPos");
		ImGui::TextUnformatted("Ctrl + mouseWheelButton : MoveCameraPos");
		ImGui::TextUnformatted("Ctrl + mouseWheel :       MoveDistance");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGui::End();
}

void AttackEditorScene::DrawRenderTexture()
{
	attackEditor_.DrawRenderTexture();
}

void AttackEditorScene::DrawPostEffect()
{
}

void AttackEditorScene::CameraUpdate()
{
}

void AttackEditorScene::LoadEnemyImGui()
{
}
