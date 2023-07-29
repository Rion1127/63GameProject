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
	//3D�I�u�W�F�N�g//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	obj_.Draw();

	PipelineManager::PreDraw("Toon", TRIANGLELIST);


	PipelineManager::PreDraw("assimp", TRIANGLELIST);

	////////////
	//�X�v���C�g//
	////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	PipelineManager::PreDraw("Particle", POINTLIST);
	ParticleManager::GetInstance()->Draw();
#ifdef _DEBUG
	ImGui::Begin("Quaternion");
	/* �����ɒǉ�������GUI������ */
	Quaternion& q = obj_.GetTransform()->quaternion_;

	ImGui::DragFloat("q.x", &q.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.y", &q.y, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.z", &q.z, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("q.w", &q.w, 0.01f, -3.1415f, 3.1415f);

	/*ImGui::SliderFloat("sideVec.x", &sideVec.x, 0.0f, 2000.0f, "x = %.3f");
	ImGui::SliderFloat("sideVec.y", &sideVec.y, 0.0f, 2000.0f, "y = %.3f");*/

	ImGui::End();
#endif // _DEBUG
}

void DebugScene::CameraUpdate()
{
	//�J�����X�V
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
