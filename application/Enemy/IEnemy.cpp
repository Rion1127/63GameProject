#include "IEnemy.h"

IEnemy::IEnemy()
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
}

void IEnemy::Update()
{
	MoveUpdate();

	obj_->Update();
	col_.center = obj_->GetTransform()->position_;
	col_.radius = obj_->GetTransform()->scale_.x;
}

void IEnemy::Draw()
{
	obj_->Draw();
}
