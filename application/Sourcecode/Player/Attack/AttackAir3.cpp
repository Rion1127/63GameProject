#include "AttackAir3.h"

AttackAir3::AttackAir3(IActor* selfActor) :
	IAttack(selfActor, 1, 30, 10, 31)
{
}

void AttackAir3::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr) {
		frontDist_ = 0.f;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		selfActor_->GetGravity()->SetGrabity({ 0,0.12f,0 });
	}
	attackVec_ = frontVec;
	//スプライン曲線計算
	spline_.SetLimitTime(attackInfo_.maxTime - 20);

	Vector3 attackBasePos = selfActor_->GetWorldTransform()->position_ + Vector3(0, 1, 0);

	std::vector<Vector3>attackVec;
	Vector3 rightVec = CalculateFrontVec().cross(Vector3(0, 1, 0));
	rightVec = rightVec.normalize();
	Vector3 playerrightPos =
		attackBasePos + rightVec * 2.f;
	attackVec.push_back(playerrightPos);
	attackVec.push_back(playerrightPos);

	Vector3 playerFrontPos =
		attackBasePos + CalculateFrontVec().normalize() * 3.f;
	attackVec.push_back(playerFrontPos);

	Vector3 playerLeftPos =
		attackBasePos + -rightVec * 2.f;
	attackVec.push_back(playerLeftPos);
	attackVec.push_back(playerLeftPos);

	spline_.SetPositions(attackVec);

	attackCol_.at(0)->col_.SetPos(playerrightPos);
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 20;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 1.f,1.f,1.f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;
}

void AttackAir3::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(selfActor_->GetWorldTransform()->rotation_.y),
		0,
		cosf(selfActor_->GetWorldTransform()->rotation_.y),
	};
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.15f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	spline_.Update(GameSpeed::GetPlayerSpeed());

	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	swordPos_ = attackCol_.at(0)->col_.center;
}
