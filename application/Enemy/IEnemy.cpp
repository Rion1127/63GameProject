#include "IEnemy.h"

IEnemy::IEnemy()
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	obj_->GetTransform()->AddPosition({ 0,6,0 });
}

void IEnemy::PreUpdate()
{
	addVec_ = { 0,0,0 };
	

	gravity_.Update();
}

void IEnemy::PostUpdate()
{
	MoveUpdate();

	addVec_ += gravity_.GetGravityValue();
	obj_->WT_.position_ += addVec_;

	ColPosUpdate();
}

void IEnemy::Draw()
{
	obj_->Draw();
}

void IEnemy::FloorColision()
{
	gravity_.SetGrabity({ 0,0,0 });
}

void IEnemy::ColPosUpdate()
{
	obj_->Update();
	col_.center = obj_->GetTransform()->position_;
	col_.radius = obj_->GetTransform()->scale_.x;
}
