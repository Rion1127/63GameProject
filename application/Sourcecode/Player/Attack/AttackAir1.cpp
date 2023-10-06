#include "AttackAir1.h"

AttackAir1::AttackAir1(IActor* selfActor) :
	IAttack(selfActor, 1, 20, 10, 21)
{
}

void AttackAir1::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr) {
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}

	spline_.SetLimitTime(attackInfo_.maxTime - 15);
	//スプライン曲線計算
	std::vector<Vector3>attackVec;
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.5f);
	Vector3 playerUpPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playerUpPos);
	attackVec.push_back(playerUpPos);

	up = {
		frontVec.normalize().x,
		frontVec.normalize().y + selfActor_->GetWorldTransform()->scale_.y * 2.5f,
		frontVec.normalize().z,
	};
	Vector3 playermiddlePos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playermiddlePos);

	Vector3 playerFrontPos =
		selfActor_->GetWorldTransform()->position_ + frontVec.normalize() * 2.5f;
	spline_.AddPosition(playerFrontPos, PosState::End);
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	spline_.SetPositions(attackVec);

	attackCol_.at(0)->col_.SetPos(playerUpPos);
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.3f,0.3f,0.3f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;
}

void AttackAir1::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(selfActor_->GetWorldTransform()->rotation_.y),
		0,
		cosf(selfActor_->GetWorldTransform()->rotation_.y),
	};
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;


	selfActor_->AddaddVec(speed);

	spline_.Update(GameSpeed::GetPlayerSpeed());

	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	swordPos_ = attackCol_.at(0)->col_.center;
}
