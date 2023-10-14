#include "Attack1.h"

Attack1::Attack1(IActor* selfActor) :
	IAttack(selfActor, 1, 35, 10, 26)
{
}

void Attack1::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr)
	{
		CalculateRotToLockOnActor();
	}
	attackVec_ = frontVec;
	//スプライン曲線計算
	SplineInit();

	attackCol_.at(0)->col_.SetPos(spline_.GetsplinePos().at(0));
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.2f,0.3f,0.2f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;

	attackSpeed_ = 0.06f;

	spline_.SetTimerType_(Spline::TimerType::Easing);
	spline_.SetEasingType_(Spline::EasingType::Circ);
	spline_.SetEasingTypeInOut_(Spline::EasingTypeInOut::In);
}

void Attack1::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();
	//プレイヤーの移動速度加算
	Vector3 speed = attackVec_ * attackSpeed_;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);
	//当たり判定をスプライン曲線で移動させる
	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	//剣の位置を代入する
	swordPos_ = attackCol_.at(0)->col_.center;
}

void Attack1::SplineInit()
{
	spline_.SetLimitTime(attackInfo_.maxTime - 20);

	std::vector<Vector3>attackVec;
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 3.f);
	Vector3 playerUpPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playerUpPos);
	attackVec.push_back(playerUpPos);

	up = {
		CalculateFrontVec().normalize().x,
		CalculateFrontVec().normalize().y + selfActor_->GetWorldTransform()->scale_.y * 3.f,
		CalculateFrontVec().normalize().z,
	};
	Vector3 playermiddlePos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(playermiddlePos);

	Vector3 playerFrontPos =
		selfActor_->GetWorldTransform()->position_ + CalculateFrontVec().normalize() * 3.f;
	playerFrontPos.y += 0.2f;
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	spline_.SetPositions(attackVec);
}
