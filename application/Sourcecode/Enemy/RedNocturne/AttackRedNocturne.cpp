#include "AttackRedNocturne.h"
#include "ParticleManager.h"
#include "ParticleFire.h"

AttackRedNocturne::AttackRedNocturne(IActor* selfActor) :
	IBullet(selfActor, 1, 25, 5, 26)
{
	
}

void AttackRedNocturne::Init()
{
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {

		colPos = selfActor_->GetWorldTransform()->position_;

		attackCol_->col_.center = colPos;
		attackCol_->col_.radius = 0.6f;
		attackCol_->damage = 10;
		//�m�b�N�o�b�N��
		attackCol_->knockPower = { 0.1f,0.3f,0.1f };
		attackCol_->knockVecY = 0.5f;
	}

	Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
	lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
	//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
	Vector3 frontVec = lockOnPos - selfActor_->GetWorldTransform()->position_;
	frontVec = frontVec.normalize();
	attackVec_ = frontVec;
	attackCol_->colObj_.SetIsVisible(false);

	bulletSpeed_ = 0.5f;
	

	Timer timer;
	timer.SetLimitTime(2);
	timer.SetTime(timer.GetLimitTimer());

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireEmitter_->particle = std::make_unique<ParticleFire>();
	fireEmitter_->addVec = { 0.5f,0.5f, 0.5f, };
	fireEmitter_->addNum = 3;
	fireEmitter_->isActive = true;
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->time = 20;
	fireEmitter_->pos = selfActor_->GetWorldTransform()->position_;
	fireEmitter_->scale = 1.f;
	ParticleManager::GetInstance()->AddParticle("fireBall", fireEmitter_);
}

void AttackRedNocturne::MoveUpdate()
{
	CalculateRotToLockOnActor(CalculateFrontVec());
	//��]��񂩂�v���C���[�ւ̃x�N�g�����擾
	Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
	lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
	Vector3 frontVec = lockOnPos - attackCol_->col_.center;
	frontVec = frontVec.normalize();
	//�����Ă����x�N�g�������X�Ƀv���C���[�̕����ɕς��Ă���
	MoveTo(frontVec,0.01f, attackVec_);

	attackCol_->col_.center += attackVec_ * bulletSpeed_;

	fireEmitter_->pos = attackCol_->col_.center;
	fireEmitter_->popCoolTime_.AddTime(1);
	if (fireEmitter_->popCoolTime_.GetIsEnd()) {
		fireEmitter_->particle->Add(
			fireEmitter_->addNum,
			fireEmitter_->time,
			fireEmitter_->pos,
			fireEmitter_->addVec,
			fireEmitter_->scale);

			fireEmitter_->popCoolTime_.Reset();
	}
}