#include "AttackAirSweep.h"
#include "Easing.h"


AttackAirSweep::AttackAirSweep(IActor* selfActor) :
	IAttack(selfActor, 1, 30, 10, 5)
{
}

void AttackAirSweep::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr) {
		frontDist_ = 1;
		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}

	attackCol_.at(0)->col_.radius = 1.3f;
	attackCol_.at(0)->knockPower = { 0.3f,0.3f,0.3f };
	colRadius_ = 1.8f;
	rotateSpeed_ = 1.7f;
}

void AttackAirSweep::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	Vector3 frontVec = CalculateFrontVec();
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.05f;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);

	rotateSpeed_ = Easing::Quint::easeIn(1.7f,2.5f,timer_.GetTimeRate());

	//�p�x���v�Z����
	float theta = (float)timer_.GetTimer() / rotateSpeed_;
	float radian = Radian(360.f / attackCol_.size());

	Vector3 colPos = {
			sinf(theta + radian) * colRadius_,
			selfActor_->GetWorldTransform()->scale_.y,
			cosf(theta + radian) * colRadius_
	};

	colPos += selfActor_->GetWorldTransform()->position_;


	attackCol_.at(0)->col_.center = colPos;
	swordPos_ = attackCol_.at(0)->col_.center;

	//�_���[�W�N�[���^�C��
	//�U�����I���܂ł̎��Ԃ��v�Z����
	int32_t limitTime = (int32_t)timer_.GetLimitTimer();
	int32_t nowTime = (int32_t)timer_.GetTimer();
	attackCol_.at(0)->damageCoolTime = limitTime - nowTime;
	
	if (timer_.GetTimeRate() < 0.8f) {
		selfActor_->GetGravity()->SetGrabity({ 0,0.1f,0 });
	}
}
