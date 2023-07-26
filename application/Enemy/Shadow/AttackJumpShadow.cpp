#include "AttackJumpShadow.h"

AttackJumpShadow::AttackJumpShadow(IActor* selfActor) :
	IAttack(selfActor,1,50,10,55)
{
}

void AttackJumpShadow::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr)
	{
		frontDist_ = 0;
		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr)
		{
			CalculateRotToLockOnActor(frontVec);
		}
		else
		{
			//��]��񂩂琳�ʃx�N�g��(2D)���擾
			frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
			};
		}
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 0.6f;
		attackCol_.at(0)->damage = 10;
		//�m�b�N�o�b�N��
		attackCol_.at(0)->knockPower = { 0.1f,0.3f,0.1f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}
	selfActor_->GetGravity()->SetGrabity({ 0,0.2f,0 });
	attackVec_ = frontVec;
	PToEVec_ =
		lockOnActor_->GetWorldTransform()->position_ - selfActor_->GetWorldTransform()->position_;
}

void AttackJumpShadow::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	attackVec_ = attackVec_.normalize();

	Vector3 speed = PToEVec_ / (float)attackInfo_.maxTime;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;

	selfActor_->AddVec(speed);

	Vector3 attackVec = attackVec_ * (selfActor_->GetWorldTransform()->scale_.x * 1.f);
	attackCol_.at(0)->col_.center = selfActor_->GetWorldTransform()->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += selfActor_->GetWorldTransform()->scale_.y;
	//�㏸���Ă���Ƃ��͍U������𖳂���
	if (selfActor_->GetGravity()->GetGravityValue().y > 0)
	{
		attackCol_.at(0)->col_.isActive = false;
	}
	//�d�͂ŉ������Ă���Ƃ��͍U�������L���ɂ���
	else
	{
		attackCol_.at(0)->col_.isActive = true;
	}
}