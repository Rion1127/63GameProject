#include "FirstAttack.h"


void FirstAttack::MoveUpdate()
{
	//attackCol_.at(0)->col_.center.z += 0.02f;
}

FirstAttack::FirstAttack() : IAttack(1,20)
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
