#include "Attack2.h"

Attack2::Attack2(IActor* selfActor) :
	IAttack(selfActor, 1, 25, 10, 26)
{
}

void Attack2::Init()
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
			CalculateRotToLockOnActor(frontVec);
		}
		else
		{
			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
			};
		}
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
	}
	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.2f,0.3f,0.2f };
	attackCol_.at(0)->knockVecY = 0.5f;

	attackVec_ = frontVec;
}

void Attack2::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = attackVec_ * 0.06f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;

	selfActor_->AddVec(speed);
	Vector3 attackVec = attackVec_ * (selfActor_->GetWorldTransform()->scale_.x * 2.f);
	attackCol_.at(0)->col_.center = selfActor_->GetWorldTransform()->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += selfActor_->GetWorldTransform()->scale_.y;
}


