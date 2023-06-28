#include "Floor.h"

Floor::Floor()
{
	floorCol_.distance = 0;
	floorCol_.normal = { 0,1,0 };

	model_ = std::move(std::make_unique<Object3d>());
	model_->SetModel(Model::CreateOBJ_uniptr("cube", true));

	model_->SetPos({ 0,-1.f,0 });
	model_->SetRot({ 0,0,0 });
	model_->SetScale({ 30,1,30 });
	model_->Update();
}

void Floor::Update()
{
	model_->Update();
}

void Floor::Draw()
{
	model_->Draw();
}
