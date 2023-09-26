#include "AttackSlide.h"

AttackSlide::AttackSlide(IActor* selfActor) :
	IAttack(selfActor, 1, 25, 10, 25)
{
}

void AttackSlide::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	frontVec_ = frontVec;
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {

		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}

		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
	}
	attackVec_ = frontVec;

	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//�m�b�N�o�b�N��
	attackCol_.at(0)->knockPower = { 0.2f,0.2f,0.2f };
	attackCol_.at(0)->knockVecY = 0.5f;

	state_ = State::Slide;

	speed_ = 0.6f;
	stateSwitchRate_ = 0.5f;
}

void AttackSlide::MoveUpdate()
{
	Vector3 moveVec{};
	Vector3 colPos{};
	if (state_ == State::Slide) {

		float rate = timer_.GetTimeRate();

		Vector3 PtoEVec{};
		if (lockOnActor_ != nullptr) {
			PtoEVec = selfActor_->GetWorldTransform()->position_ - lockOnActor_->GetWorldTransform()->position_;
		}
		float length = PtoEVec.length();

		if (length < 4.f ||
			rate >= stateSwitchRate_)
		{
			state_ = State::SpeedDown;
			float time = timer_.GetLimitTimer() * rate;
			timer_.SetTime(time);
		}
	}
	else if (state_ == State::SpeedDown) {
		speed_ -= 0.07f;
		speed_ = Max(speed_, 0.f);
	}
	moveVec = frontVec_.normalize() * speed_;
	selfActor_->AddaddVec(moveVec);

	colPos = selfActor_->GetWorldTransform()->position_;
	colPos += frontVec_.normalize() * 2.5f;
	colPos.y += 0.7f;
	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->damageCoolTime = (int32_t)timer_.GetLimitTimer() - (int32_t)timer_.GetTimer() + 1;
	swordPos_ = attackCol_.at(0)->col_.center;
}