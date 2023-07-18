#include "DebugScene.h"

#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "Camera.h"

DebugScene::~DebugScene()
{

}

void DebugScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	lightManager_ = std::make_shared<LightManager>();

	Model::SetLight(lightManager_->GetLightGroup());

	obj_.SetModel(Model::CreateOBJ_uniptr("gumiShip", true));
	obj_.GetTransform()->SetRotType(RotType::Quaternion);
}

void DebugScene::Update()
{
	CameraUpdate();

	obj_.Update();

	lightManager_->DebugUpdate();
	ParticleManager::GetInstance()->Update();
}

void DebugScene::Draw()
{
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	obj_.Draw();

	PipelineManager::PreDraw("Toon", TRIANGLELIST);


	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//スプライト//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();

	ImGui::Begin("Quaternion");
	/* ここに追加したいGUIを書く */
	Quaternion& q = obj_.GetTransform()->quaternion_;

	
	ImGui::DragFloat("q.x", &q.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.y", &q.y, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.z", &q.z, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.w", &q.w, 0.01f, -3.1415f, 3.1415f);

	

	/*ImGui::SliderFloat("sideVec.x", &sideVec.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("sideVec.y", &sideVec.y, 0.0f, 2000.0f, "y = %.3f");*/

	ImGui::End();
}

void DebugScene::CameraUpdate()
{
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
