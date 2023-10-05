#include "Attack2.h"

Attack2::Attack2(IActor* selfActor) :
	IAttack(selfActor, 1, 25, 10, 26)
{
}

void Attack2::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr)
	{
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr)
		{
			CalculateRotToLockOnActor(frontVec);
		}
	}
	attackVec_ = frontVec;

	spline_.SetLimitTime(attackInfo_.maxTime - 15);
	//スプライン曲線計算(剣の挙動)
	Vector3 attackBasePos = selfActor_->GetWorldTransform()->position_ + Vector3(0, 1, 0);

	std::vector<Vector3>attackVec;
	Vector3 playerFrontPos =
		attackBasePos + CalculateFrontVec().normalize();
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	Vector3 endPos =
		attackBasePos + CalculateFrontVec().normalize() * 3.f;
	attackVec.push_back(endPos);
	attackVec.push_back(endPos);

	spline_.SetPositions(attackVec);

	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.2f,0.3f,0.2f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;
}

void Attack2::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = attackVec_ * 0.06f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	

	spline_.Update(GameSpeed::GetPlayerSpeed());

	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	swordPos_ = attackCol_.at(0)->col_.center;
}


