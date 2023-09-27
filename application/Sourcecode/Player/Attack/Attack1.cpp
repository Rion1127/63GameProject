#include "Attack1.h"

Attack1::Attack1(IActor* selfActor) :
	IAttack(selfActor, 1, 25, 10, 26)
{
}

void Attack1::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	if (selfActor_ != nullptr)
	{
		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr)
		{
			CalculateRotToLockOnActor(frontVec);
		}
	}
	attackVec_ = frontVec;
	//�X�v���C���Ȑ��v�Z
	SplineInit();

	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//�m�b�N�o�b�N��
	attackCol_.at(0)->knockPower = { 0.2f,0.3f,0.2f };
	attackCol_.at(0)->knockVecY = 0.5f;
	swordPos_ = attackCol_.at(0)->col_.center;

	attackSpeed_ = 0.06f;
}

void Attack1::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	attackVec_ = attackVec_.normalize();
	//�v���C���[�̈ړ����x���Z
	Vector3 speed = attackVec_ * attackSpeed_;
	float timerate = 1.f - timer_.GetTimeRate();
	speed *= timerate;

	selfActor_->AddaddVec(speed);
	//�����蔻����X�v���C���Ȑ��ňړ�������
	spline_.Update(GameSpeed::GetPlayerSpeed());
	attackCol_.at(0)->col_.center = spline_.GetNowPoint();
	//���̈ʒu��������
	swordPos_ = attackCol_.at(0)->col_.center;
}

void Attack1::SplineInit()
{
	spline_.SetLimitTime(attackInfo_.maxTime - 20);

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
		selfActor_->GetWorldTransform()->position_ + CalculateFrontVec().normalize() * 3.f;
	playerFrontPos.y += 0.2f;
	attackVec.push_back(playerFrontPos);
	attackVec.push_back(playerFrontPos);

	spline_.SetPositions(attackVec);
}
