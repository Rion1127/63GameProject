#include "IActor.h"
#include "Util.h"
#include "ParticleHitAttack.h"
#include "GameSpeed.h"

/**
 * @file IActor.cpp
 * @brief インターフェースクラス
 * @brief プレイヤーや敵・モブ等を作る際に継承するクラス
 */

void IActor::ObjUpdate()
{
	HitStopUpdate();
	if (GetIsHitStopping() == false) {
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
	}
	//もし敵が地面の下へ言ったしまった場合
	if (obj_->WT_.position_.y <= -10) {
		obj_->WT_.position_.y = 1;
	}

	obj_->Update();
}

IActor::IActor(ActorType type)
{
	knockDecreaseValue = 0.04f;
	type_ = type;
}
