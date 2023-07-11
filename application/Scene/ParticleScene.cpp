#include "ParticleScene.h"
#include "SceneManager.h"
#include "GameScene.h"

ParticleScene::~ParticleScene()
{
}

void ParticleScene::Ini()
{
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	//デバッグカメラ初期化
	debugCamera.Update();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update(CameraMode::LookAT);
	//useVP = &gameCamera;
	useVP = debugCamera.GetCamera();
	useVP->SetOriginalPos();
	
	lightGroup = std::make_shared<LightGroup>();
	lightGroup->Init();
	Model::SetLight(lightGroup);
	
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	

	//const wchar_t* modelFile = L"Resources/boneTest/testCube.gltf";
	////  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	////  L"Resources/FBX/untitled.glb"
	//ImportSettings importSetting = {
	//	modelFile,
	//	meshes,
	//	false,
	//	true
	//};
	//testModel_.Create(modelFile);
	//assimpObj_.SetModel(&testModel_);
}

void ParticleScene::Update()
{
	Camera::scurrent_->eye_ = debugCamera.GetCamera()->eye_;
	Camera::scurrent_->up_ = debugCamera.GetCamera()->up_;
	Camera::scurrent_->target_ = debugCamera.GetCamera()->target_;
	Camera::scurrent_->Update(CameraMode::LookAT);
	//カメラ更新
	if (Key::PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update(CameraMode::LookAT);


	WT_.Update();


	testWT_.Update();

	lightGroup->SetDirLightColor(0, { 1,1,1 });
	
	lightGroup->Update();

	assimpObj_.Update();

}

void ParticleScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	lightGroup->Draw(3);
	
	//assimpObj_.Draw();

	///////////////////
	/////パーティクル////
	///////////////////
	
}
