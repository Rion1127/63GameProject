#include "IEnemy.h"

void IEnemy::PreUpdate()
{
	addVec_ = { 0,0,0 };

	gravity_.Update();

	damegeCoolTime_.AddTime(1);
}

void IEnemy::PostUpdate()
{
	MoveUpdate();

	addVec_ += gravity_.GetGravityValue();
	addVec_ += knockVec_;
	//ノックバックのベクトルを0にしていく
	MoveTo({0,0,0},0.05f, knockVec_);
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

void IEnemy::HitPlayerAttack(Vector3 knockVec, float damageValue, int32_t cooltime)
{
	if (damegeCoolTime_.GetIsEnd()) {
		knockVec_ = knockVec;
		damegeCoolTime_.SetLimitTime(cooltime);
		damegeCoolTime_.Reset();
	}
	
}
