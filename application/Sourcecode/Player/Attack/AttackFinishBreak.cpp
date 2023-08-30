#include "AttackFinishBreak.h"
#include "ParticleExplosion.h"
#include "ParticleManager.h"

AttackFinishBreak::AttackFinishBreak(IActor* selfActor) : 
	IAttack(selfActor,3,100,20,60)
{
}

void AttackFinishBreak::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr)
	{
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr)
		{
			CalculateRotToLockOnActor(frontVec);
		}

		frontVec = frontVec.normalize();
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
	}
	attackVec_ = frontVec;

	//スプライン曲線計算
	splineTime_ = 6;
	SplineUpdate();

	hitNum_ = 5;

	int32_t coolTime = attackInfo_.maxTime - splineTime_ * ((int32_t)spline_.GetsplinePos().size() - 2);
	coolTime /= hitNum_;

	for (uint32_t i = 0; i < attackCol_.size(); i++) {
		attackCol_.at(i)->col_.center = colPos;
		attackCol_.at(i)->col_.radius = 1.2f;
		attackCol_.at(i)->damage = 3;
		attackCol_.at(i)->damageCoolTime = coolTime + 1;
		//ノックバック力
		attackCol_.at(i)->knockPower = { 0.f,1.0f,0.f };
		attackCol_.at(i)->knockVecY = 0.3f;
	}
	colRadius_ = 1.8f;
	rotateSpeed_ = 10.f;
	isParticleAdd_ = true;

	spline_.Update();
	swordPos_ = spline_.GetNowPoint();
}

void AttackFinishBreak::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	if (spline_.GetIndex() <= 1) {
		splineTime_ = 15;
	}
	else {
		splineTime_ = 2;
	}
	SplineUpdate();
	spline_.Update();
	//剣を振り終わったら攻撃の判定を有効にする
	if (spline_.GetisEnd()) {
		int32_t changeColStateTime = attackCol_.at(0)->damageCoolTime * (hitNum_ - 1);
		for (uint32_t i = 0; i < attackCol_.size(); i++) {
			//当たり判定有効
			attackCol_.at(i)->col_.isActive = true;
			//角度を計算する
			float theta = (float)timer_.GetTimer() / rotateSpeed_;
			float radian = Radian(360.f / attackCol_.size());
			Vector3 colPos = {
				sinf(theta + (i * radian)) * colRadius_,
				selfActor_->GetWorldTransform()->scale_.y,
				cosf(theta + (i * radian)) * colRadius_
			};
			attackCol_.at(i)->col_.center =
				selfActor_->GetWorldTransform()->position_ + colPos;
			//最後の一発は強めの攻撃にする
			if (changeColStateTime < timer_.GetTimer()) {
				attackCol_.at(i)->damage = 15;
				//ノックバック力
				attackCol_.at(i)->knockPower = { 1.0f,1.0f,1.0f };
				attackCol_.at(i)->knockVecY = 0.5f;
			}
			//パーティクル追加
			if (isParticleAdd_) {
				emitter_[i] = std::make_shared<OneceEmitter>();
				emitter_[i]->particle = std::make_unique<ParticleExplosion>();
				emitter_[i]->addNum = 1;
				emitter_[i]->time = changeColStateTime;
				emitter_[i]->pos = attackCol_.at(i)->col_.center;
				emitter_[i]->addVec = Vector3(0, 0, 0);
				emitter_[i]->scale = 1.2f;
				ParticleManager::GetInstance()->
					AddParticle("Explosion", emitter_[i]);
			}
			emitter_[i]->pos = attackCol_.at(i)->col_.center;
		}
		isParticleAdd_ = false;
	}
	else {
		for (uint32_t i = 0; i < attackCol_.size(); i++) {
			attackCol_.at(i)->col_.isActive = false;
		}
	}
	swordPos_ = spline_.GetNowPoint();
}

void AttackFinishBreak::SplineUpdate()
{
	//スプライン曲線計算
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
		selfActor_->GetWorldTransform()->position_ + CalculateFrontVec().normalize();
	playerFrontPos.y += 0.2f;
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	spline_.SetPositions(attackVec);

	

	spline_.SetLimitTime(splineTime_);
}
