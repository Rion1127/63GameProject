#include "IEnemy.h"

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
