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

	if (type_ == ActorType::Player) addVec_ *= GameSpeed::GetPlayerSpeed();
	else if (type_ == ActorType::Enemy) addVec_ *= GameSpeed::GetEnemySpeed();

	obj_->WT_.position_ += addVec_;
	obj_->Update();
}

IActor::IActor(ActorType type)
{
	type_ = type;
}
