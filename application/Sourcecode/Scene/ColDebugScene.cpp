#include "ColDebugScene.h"

ColDebugScene::~ColDebugScene()
{
}

void ColDebugScene::Ini()
{
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());

	col1Obj_ = std::move(std::make_unique<Object3d>());
	col1Obj_->SetModel(Model::CreateOBJ_uniptr("uvsphere", false, false));

	col2Obj_ = std::move(std::make_unique<Object3d>());
	col2Obj_->SetModel(Model::CreateOBJ_uniptr("uvsphere", false, false));

	Camera::scurrent_ = debugCamera_.GetCamera();
}

void ColDebugScene::Update()
{
	lightManager_->Update();

	float speed = 0.1f;
	if (Key::PushKey(DIK_LSHIFT))speed /= 10.f;

	Vector3 pos1 = col1Obj_->GetPos();
	Vector3 pos2 = col2Obj_->GetPos();

	if (Key::PushKey(DIK_W)) pos1.y += speed;
	if (Key::PushKey(DIK_S)) pos1.y -= speed;
	if (Key::PushKey(DIK_D)) pos1.x += speed;
	if (Key::PushKey(DIK_A)) pos1.x -= speed;
	if (Key::PushKey(DIK_Q)) pos1.z += speed;
	if (Key::PushKey(DIK_E)) pos1.z -= speed;

	if (Key::PushKey(DIK_I)) pos2.y += speed;
	if (Key::PushKey(DIK_K)) pos2.y -= speed;
	if (Key::PushKey(DIK_L)) pos2.x += speed;
	if (Key::PushKey(DIK_J)) pos2.x -= speed;
	if (Key::PushKey(DIK_U)) pos2.z += speed;
	if (Key::PushKey(DIK_O)) pos2.z -= speed;

	col2Obj_->SetScale(Vector3(1,0.5f,1));

	col1_.center = pos1;
	col1_.radius = 1.f;
	col2_.center = pos2;
	col2_.radius = 1.f;

	if (BallCollision(col1_, 0.5f, col2_, 1.f)) {
		col1Obj_->SetAmbient("uvsphere", Vector3(255, 0, 0));
	}
	else {
		col1Obj_->SetAmbient("uvsphere", Vector3(255, 255, 255));
	}


	col1Obj_->SetPos(pos1);
	col2Obj_->SetPos(pos2);
	col1Obj_->Update();
	col2Obj_->Update();

	//カメラ更新
	debugCamera_.Update();
}

void ColDebugScene::Draw()
{
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	col1Obj_->Draw();
	col2Obj_->Draw();
}

void ColDebugScene::DrawRenderTexture()
{
}

void ColDebugScene::DrawPostEffect()
{
}

void ColDebugScene::CameraUpdate()
{
}
