#include "IBullet.h"

/**
 * @file IBullet.cpp
 * @brief インターフェースクラス
 * @brief 弾・遠距離魔法を作る際に継承するクラス
 */

IBullet::IBullet(IActor* selfActor, int32_t maxTime, int32_t damage, float damageCoolTime, int32_t costMP) :
	selfActor_(selfActor)
{

	attackCol_ = std::move(std::make_unique<AttackCol>());
	attackCol_->colObj_.SetModel(Model::CreateOBJ_uniptr("sphere", false,false));
	attackCol_->colObj_.SetAmbient("sphere", { 1.0f, 0, 0 });
	attackCol_->colObj_.SetIsVisible(false);
	attackCol_->damageCoolTime = damageCoolTime;
	attackCol_->damage = damage;

	attackInfo_.maxTime = maxTime;
	spline_.SetIsStart(true);
	isDead_ = false;
	aliveTimer_.SetLimitTime(maxTime);

	costMP_ = costMP;
}

void IBullet::Update()
{
	aliveTimer_.AddTime(1);
	if (aliveTimer_.GetIsEnd()) {
		isDead_ = true;
	}
	if (selfActor_ != nullptr) {
		MoveUpdate();
	}

	attackCol_->colObj_.WT_.position_ = attackCol_->col_.center;
	attackCol_->colObj_.WT_.scale_ = {
		attackCol_->col_.radius,
		attackCol_->col_.radius,
		attackCol_->col_.radius,
	};
	attackCol_->colObj_.Update();
}

void IBullet::DrawCol()
{
	attackCol_->colObj_.Draw();
}

Vector3 IBullet::CalculateFrontVec()
{
	Vector3 frontVec = { 0,0,0 };
	if (lockOnActor_ != nullptr)
	{
		Vector3& lockOnPos = lockOnActor_->GetWorldTransform()->position_;
		//ロックオンしている敵へのベクトルをとる
		frontVec = {
			lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
			0,
			lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
		};
	}
	else
	{
		frontVec = RotateVector(Vector3(0, 0, 1), selfActor_->GetAxisY());
	}
	return frontVec.normalize();
}

void IBullet::CalculateRotToLockOnActor(const Vector3& frontVec)
{
	Vector2 frontVec2 = {
		frontVec.x,
		frontVec.z
	};
	float rotY = Radian(Vec2Angle(frontVec2));
	selfActor_->GetWorldTransform()->rotation_ = { 0,rotY ,0 };
}
