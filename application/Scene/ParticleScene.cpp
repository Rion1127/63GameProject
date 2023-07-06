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

	//�f�o�b�O�J����������
	debugCamera.Update();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetCamera();
	useVP->SetOriginalPos();
	
	lightGroup = std::make_shared<LightGroup>();
	lightGroup->Init();
	Model::SetLight(lightGroup);
	AssimpModel::SetLightGroup(lightGroup.get());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	

	const wchar_t* modelFile = L"Resources/boneTest/testCube.gltf";
	//  L"Resources/FBX/Alica/Alicia_solid_Unity.FBX"
	//  L"Resources/FBX/untitled.glb"
	ImportSettings importSetting = {
		modelFile,
		meshes,
		false,
		true
	};
	testModel_.Create(modelFile);
	assimpObj_.SetModel(&testModel_);
}

void ParticleScene::Update()
{
	Camera::scurrent_.eye_ = debugCamera.GetCamera()->eye_;
	Camera::scurrent_.up_ = debugCamera.GetCamera()->up_;
	Camera::scurrent_.target_ = debugCamera.GetCamera()->target_;
	Camera::scurrent_.Update();
	//�J�����X�V
	if (Key::PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update();


	testWT_.Update();

	lightGroup->SetDirLightColor(0, { 1,1,1 });
	
	lightGroup->Update();

	assimpObj_.Update();

}

void ParticleScene::Draw()
{
	////////////////
	//3D�I�u�W�F�N�g//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	lightGroup->Draw(3);
	
	//assimpObj_.Draw();

	///////////////////
	/////�p�[�e�B�N��////
	///////////////////
	
}
