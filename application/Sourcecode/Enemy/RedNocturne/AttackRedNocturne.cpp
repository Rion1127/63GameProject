#include "AttackRedNocturne.h"

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
	attackCol_->colObj_.SetIsVisible(true);

	bulletSpeed_ = 0.5f;
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
}