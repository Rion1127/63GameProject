#include "Attack3.h"

Attack3::Attack3() : IAttack(1, 30)
{
	frontDist_ = 5.f;
	if (splayerInfo_ != nullptr) {
		Vector3 colPos;

		//��]��񂩂琳�ʃx�N�g��(2D)���擾
		Vector3 frontVec = {
			sinf(splayerInfo_->WT->rotation_.y),
			0,
			cosf(splayerInfo_->WT->rotation_.y),
		};

		Vector3 frontDist = frontVec * frontDist_;
		frontDist.y = 0;

		colPos = splayerInfo_->WT->position_ + frontDist;
		colPos.y += 1;
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 3.f;
	}
}

void Attack3::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	Vector3 frontVec = {
		sinf(splayerInfo_->WT->rotation_.y),
		0,
		cosf(splayerInfo_->WT->rotation_.y),
	};
	frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;

	//maxTime - 10�̎��ԕ��v���C���[��O�ɐi�߂�
	if (attackInfo_.nowTime < attackInfo_.maxTime - 10) {
		*splayerInfo_->addVec_ += speed;
		attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + frontVec * 5.f;
	}
}
