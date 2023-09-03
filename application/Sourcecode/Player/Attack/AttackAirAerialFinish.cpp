#include "AttackAirAerialFinish.h"

AttackAirAerialFinish::AttackAirAerialFinish(IActor* selfActor) : 
	IAttack(selfActor,1,70,10,5)
{
}

void AttackAirAerialFinish::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}

	FirstAttackInit();
	index_ = 0;
}

void AttackAirAerialFinish::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(selfActor_->GetWorldTransform()->rotation_.y),
		0,
		cosf(selfActor_->GetWorldTransform()->rotation_.y),
	};
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.05f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	spline_.Update();

	if (spline_.GetisEnd()) {
		spline_.Reset();
		index_++;
		if (index_ == 1) {
			SecondAttackInit();
		}
		else if (index_ == 2) {
			ThirdAttackInit();
		}
		else if (index_ == 3) {
			FourthAttackInit();
		}
	}
	if (index_ < 4) {
		swordPos_ = attackCol_.at(0)->col_.center;
		attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	}
	int32_t limitTime = spline_.GetTimer().GetLimitTimer() + 7;
	int32_t nowTime = spline_.GetTimer().GetTimer();
	attackCol_.at(0)->damageCoolTime = limitTime - nowTime;
	selfActor_->GetGravity()->SetGrabity({ 0,0,0 });

}

void AttackAirAerialFinish::FirstAttackInit()
{
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.1f,0.1f,0.1f };
	attackCol_.at(0)->knockVecY = 0.5f;


	//スプライン曲線計算
	std::vector<Vector3>attackVec;
	Vector3 frontVec = CalculateFrontVec();
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.5f);

	//正面斜め下
	up = {
		frontVec.normalize().x / 2.f,
		frontVec.normalize().y / 2.f - selfActor_->GetWorldTransform()->scale_.y * 0.5f,
		frontVec.normalize().z / 2.f,
	};
	Vector3 fourthPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(fourthPos);
	attackVec.push_back(fourthPos);
	//正面
	Vector3 thirdPos =
		selfActor_->GetWorldTransform()->position_ + frontVec.normalize() * 2.5f;
	attackVec.push_back(thirdPos);
	//斜め上
	up = {
		frontVec.normalize().x,
		frontVec.normalize().y + selfActor_->GetWorldTransform()->scale_.y * 2.5f,
		frontVec.normalize().z,
	};
	Vector3 secondPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(secondPos);
	//上
	Vector3 endPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(endPos);
	attackVec.push_back(endPos);

	spline_.SetPositions(attackVec);

	int32_t splineTime = attackInfo_.maxTime / 4;
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime(splineTime);


	swordPos_ = attackCol_.at(0)->col_.center;
}

void AttackAirAerialFinish::SecondAttackInit()
{
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.1f,0.1f,0.1f };
	attackCol_.at(0)->knockVecY = 0.5f;

	//スプライン曲線計算
	std::vector<Vector3>attackVec;
	Vector3 frontVec = CalculateFrontVec();
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.5f);
	//上
	Vector3 endPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(endPos);
	attackVec.push_back(endPos);
	//斜め上
	up = {
		frontVec.normalize().x,
		frontVec.normalize().y + selfActor_->GetWorldTransform()->scale_.y * 2.5f,
		frontVec.normalize().z,
	};
	Vector3 secondPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(secondPos);
	//正面
	Vector3 thirdPos =
		selfActor_->GetWorldTransform()->position_ + frontVec.normalize() * 2.5f;
	attackVec.push_back(thirdPos);
	//正面斜め下
	up = {
		frontVec.normalize().x / 2.f,
		frontVec.normalize().y / 2.f - selfActor_->GetWorldTransform()->scale_.y * 0.5f,
		frontVec.normalize().z / 2.f,
	};
	Vector3 fourthPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(fourthPos);
	attackVec.push_back(fourthPos);

	spline_.SetPositions(attackVec);
	int32_t splineTime = attackInfo_.maxTime / 4;
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime(splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;
}

void AttackAirAerialFinish::ThirdAttackInit()
{
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.1f,0.1f,0.1f };
	attackCol_.at(0)->knockVecY = 0.5f;

	
	//スプライン曲線計算
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
	spline_.AddPosition(playerFrontPos, PosState::Middle);
	attackVec.push_back(playerFrontPos);

	Vector3 playerLeftPos =
		attackBasePos + -rightVec * 2.f;
	attackVec.push_back(playerLeftPos);
	attackVec.push_back(playerLeftPos);

	spline_.SetPositions(attackVec);
	int32_t splineTime = attackInfo_.maxTime / 4;
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime(splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;
}

void AttackAirAerialFinish::FourthAttackInit()
{
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.1f,0.1f,0.1f };
	attackCol_.at(0)->knockVecY = 0.5f;


	//スプライン曲線計算
	std::vector<Vector3>attackVec;
	Vector3 frontVec = CalculateFrontVec();
	Vector3 up = Vector3(0, 1, 0) * (selfActor_->GetWorldTransform()->scale_.y * 2.5f);
	//上
	Vector3 endPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(endPos);
	attackVec.push_back(endPos);
	//斜め上
	up = {
		frontVec.normalize().x,
		frontVec.normalize().y + selfActor_->GetWorldTransform()->scale_.y * 2.5f,
		frontVec.normalize().z,
	};
	Vector3 secondPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(secondPos);
	//正面
	Vector3 thirdPos =
		selfActor_->GetWorldTransform()->position_ + frontVec.normalize() * 2.5f;
	attackVec.push_back(thirdPos);
	//正面斜め下
	up = {
		frontVec.normalize().x / 2.f,
		frontVec.normalize().y / 2.f - selfActor_->GetWorldTransform()->scale_.y * 0.5f,
		frontVec.normalize().z / 2.f,
	};
	Vector3 fourthPos =
		selfActor_->GetWorldTransform()->position_ + up;
	attackVec.push_back(fourthPos);
	attackVec.push_back(fourthPos);

	spline_.SetPositions(attackVec);
	int32_t splineTime = attackInfo_.maxTime / 4;
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime(splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;
}
