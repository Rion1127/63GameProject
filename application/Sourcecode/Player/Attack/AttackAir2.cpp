#include "AttackAir2.h"

AttackAir2::AttackAir2(IActor* selfActor) :
	IAttack(selfActor, 1, 20, 10, 21)
{
}

void AttackAir2::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr) {
		CalculateRotToLockOnActor();
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}
	attackVec_ = frontVec;

	spline_.SetLimitTime(attackInfo_.maxTime - 15);
	//スプライン曲線計算
	std::vector<Vector3>attackVec;
	Vector3 up = Vector3(0, -1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 0.5f);
	Vector3 playerDownPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playerDownPos);
	attackVec.push_back(playerDownPos);

	up = {
		frontVec.normalize().x,
		frontVec.normalize().y + selfActor_->GetWorldTransform()->scale_.y * -1.5f,
		frontVec.normalize().z,
	};
	Vector3 playermiddlePos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playermiddlePos);

	Vector3 playerFrontPos =
		selfActor_->GetWorldTransform()->position_ + frontVec.normalize() * 2.5f;
	attackVec.push_back(playerFrontPos);

	up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.f);
	Vector3 playerUpPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playerUpPos);
	attackVec.push_back(playerUpPos);

	spline_.SetPositions(attackVec);

	attackCol_.at(0)->col_.SetPos(playerDownPos);
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.3f,0.3f,0.3f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;

	spline_.SetTimerType_(Spline::TimerType::Easing);
	spline_.SetEasingType_(Spline::EasingType::Sine);
	spline_.SetEasingTypeInOut_(Spline::EasingTypeInOut::In);
}

void AttackAir2::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec =
		RotateVector(Vector3(0, 0, 1), selfActor_->GetWorldTransform()->quaternion_);
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);



	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackCol_.at(0)->damageCoolTime = (uint32_t)timer_.GetLimitTimer() - (int32_t)timer_.GetTimer();
	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	swordPos_ = attackCol_.at(0)->col_.center;
}
