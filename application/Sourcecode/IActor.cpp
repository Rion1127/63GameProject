#include "IActor.h"
#include "Util.h"
#include "ParticleHitAttack.h"
#include "GameSpeed.h"

void IActor::ObjUpdate()
{
	addVec_ += gravity_.GetGravityValue();
	addVec_ += knockVec_;
	//ノックバックのベクトルを0にしていく
	MoveTo({ 0,0,0 }, knockDecreaseValue, knockVec_);

	if (isCanMove_ == false)
	{
		addVec_ = { 0,0,0 };
	}
	obj_->WT_.position_ += addVec_ * GameSpeed::GetGameSpeed();
	obj_->Update();
}

IActor::IActor()
{
	
}
