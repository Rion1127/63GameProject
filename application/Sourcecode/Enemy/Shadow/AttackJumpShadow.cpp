#include "AttackJumpShadow.h"

/**
 * @file AttackJumpShadow.cpp
 * @brief シャドウのジャンプ攻撃攻撃
 */

AttackJumpShadow::AttackJumpShadow(IActor* selfActor) :
	IAttack(selfActor, 1, 50, 10, 55)
{
}

void AttackJumpShadow::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr)
	{
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr)
		{
			CalculateRotToLockOnActor();
		}
		else
		{
			CalculateRotToLockOnActor();
		}
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 0.6f;
		attackCol_.at(0)->damage = 10;
		//ノックバック力
		attackCol_.at(0)->knockPower = { 0.1f,0.3f,0.1f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}
	selfActor_->GetGravity()->SetGrabity({ 0,0.2f,0 });
	attackVec_ = frontVec;
	PToEVec_ =
		lockOnActor_->GetWorldTransform()->position_ - selfActor_->GetWorldTransform()->position_;
}

void AttackJumpShadow::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = PToEVec_ / (float)attackInfo_.maxTime;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	Vector3 attackVec = attackVec_ * (selfActor_->GetWorldTransform()->scale_.x * 1.f);
	attackCol_.at(0)->col_.center = selfActor_->GetWorldTransform()->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += selfActor_->GetWorldTransform()->scale_.y;
	//上昇しているときは攻撃判定を無くす
	if (selfActor_->GetGravity()->GetGravityValue().y > 0)
	{
		attackCol_.at(0)->col_.isActive = false;
	}
	//重力で下がっているときは攻撃判定を有効にする
	else
	{
		attackCol_.at(0)->col_.isActive = true;
	}
}
