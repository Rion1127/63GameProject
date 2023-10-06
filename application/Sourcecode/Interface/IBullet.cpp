#include "IBullet.h"


IBullet::IBullet(IActor* selfActor, int32_t maxTime, int32_t damage, int32_t damageCoolTime, int32_t costMP) :
	selfActor_(selfActor)
{

	attackCol_ = std::move(std::make_unique<AttackCol>());
	attackCol_->colObj_.SetModel(Model::CreateOBJ_uniptr("sphere", false));
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
	if (IBullet::lockOnActor_ != nullptr)
	{
		Vector3& lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
		//ロックオンしている敵へのベクトルをとる
		frontVec = {
			lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
			0,
			lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
		};
	}
	else
	{
		frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
		};
	}
	return frontVec;
}

void IBullet::CalculateRotToLockOnActor(Vector3 frontVec)
{
	Vector2 frontVec2 = {
		frontVec.x,
		frontVec.z
	};
	float rotY = Radian(Vec2Angle(frontVec2));
	selfActor_->GetWorldTransform()->rotation_ = { 0,rotY ,0 };
}
