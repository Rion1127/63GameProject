#include "Attack1.h"

Attack1::Attack1() : IAttack(1, 20)
{
	frontDist_ = 5.f;
	if (playerInfo_ != nullptr) {
		Vector3 colPos;

		//��]��񂩂琳�ʃx�N�g��(2D)���擾
		Vector3 frontVec = {
			sinf(playerInfo_->WT->rotation_.y),
			0,
			cosf(playerInfo_->WT->rotation_.y),
		};

		Vector3 frontDist = frontVec * frontDist_;
		frontDist.y = 0;

		colPos = playerInfo_->WT->position_ + frontDist;

		attackCol_.at(0)->col_.center = colPos;
	}
}

void Attack1::MoveUpdate()
{
	//attackCol_.at(0)->col_.center.z += 0.02f;
}

void Attack1::PlayerMoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	Vector3 frontVec = {
		sinf(playerInfo_->WT->rotation_.y),
		0,
		cosf(playerInfo_->WT->rotation_.y),
	};
	frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;

	//maxTime - 10�̎��ԕ��v���C���[��O�ɐi�߂�
	if (attackInfo_.nowTime < attackInfo_.maxTime - 10) {
		*playerInfo_->addVec_ += speed;
		attackCol_.at(0)->col_.center += *playerInfo_->addVec_;
	}
}

