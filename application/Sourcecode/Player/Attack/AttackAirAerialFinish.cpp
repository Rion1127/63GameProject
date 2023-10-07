#include "AttackAirAerialFinish.h"
#include "RRandom.h"
#include "mSound.h"

AttackAirAerialFinish::AttackAirAerialFinish(IActor* selfActor) :
	IAttack(selfActor, 1, 70, 10, 5)
{
}

void AttackAirAerialFinish::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr) {
		frontDist_ = 0;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		Vector2 vec = { CalculateFrontVec().x,CalculateFrontVec().z };
		float rot = Vec2Angle(vec);
		Vector3 vecY = { 0, 1, 0 };
		auto axisY = MakeAxisAngle(vecY, Radian(rot));
		selfActor_->GetWorldTransform()->SetQuaternion(axisY);
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}

	FirstAttackInit();
	index_ = 0;
}

void AttackAirAerialFinish::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = CalculateFrontVec();
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.08f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	spline_.Update(GameSpeed::GetPlayerSpeed());

	if (spline_.GetisEnd()) {
		spline_.Reset();
		index_++;
		damegeCooltime_ = 0;
		float picth = RRandom::RandF(0.7f, 1.5f);
		if (index_ == 1) {
			SecondAttackInit();
			SoundManager::Play("SwingSE", false, 0.3f, picth);
		}
		else if (index_ == 2) {
			ThirdAttackInit();
			SoundManager::Play("SwingSE", false, 0.3f, picth);
		}
		else if (index_ == 3) {
			FourthAttackInit();
			SoundManager::Play("SwingSE", false, 0.3f, picth);
		}
	}

	if (index_ < 4) {
		damegeCooltime_++;
		swordPos_ = attackCol_.at(0)->col_.center;
		attackCol_.at(0)->col_.center = spline_.GetNowPoint();
		float limitTime = spline_.GetTimer().GetLimitTimer() * (float)(spline_.GetsplinePos().size());
		float nowTime = damegeCooltime_;
		attackCol_.at(0)->damageCoolTime = (int32_t)(limitTime - nowTime);
		selfActor_->GetGravity()->SetGrabity({ 0,0,0 });
	}
	else {
		attackCol_.at(0)->col_.isActive = false;
	}

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

	int32_t splineTime = (int32_t)(attackInfo_.maxTime / 4);
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime((float)splineTime);

	attackCol_.at(0)->col_.center = fourthPos;
	swordPos_ = attackCol_.at(0)->col_.center;

	float limitTime = spline_.GetTimer().GetLimitTimer() * (float)(spline_.GetsplinePos().size());
	float nowTime = damegeCooltime_;
	attackCol_.at(0)->damageCoolTime = (int32_t)(limitTime - nowTime);
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
	int32_t splineTime = (int32_t)(attackInfo_.maxTime / 4);
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime((float)splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;

	float limitTime = spline_.GetTimer().GetLimitTimer() * (float)(spline_.GetsplinePos().size());
	float nowTime = damegeCooltime_;
	attackCol_.at(0)->damageCoolTime = (int32_t)(limitTime - nowTime);
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
	int32_t splineTime = (int32_t)(attackInfo_.maxTime / 4);
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime((float)splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;

	float limitTime = spline_.GetTimer().GetLimitTimer() * (float)(spline_.GetsplinePos().size());
	float nowTime = damegeCooltime_;
	attackCol_.at(0)->damageCoolTime = (int32_t)(limitTime - nowTime);
}

void AttackAirAerialFinish::FourthAttackInit()
{
	attackCol_.at(0)->col_.radius = 0.8f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.8f,0.1f,0.8f };
	attackCol_.at(0)->knockVecY = 1.0f;


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
	int32_t splineTime = (int32_t)(attackInfo_.maxTime / 4);
	int32_t splineSize = ((uint32_t)spline_.GetsplinePos().size() - 2);
	splineTime /= splineSize;
	spline_.SetLimitTime((float)splineTime);

	swordPos_ = attackCol_.at(0)->col_.center;

	float limitTime = spline_.GetTimer().GetLimitTimer() * (float)(spline_.GetsplinePos().size());
	float nowTime = damegeCooltime_;
	attackCol_.at(0)->damageCoolTime = (int32_t)(limitTime - nowTime);
}
