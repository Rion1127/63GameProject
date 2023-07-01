#include "Attack1.h"

Attack1::Attack1() : IAttack(1, 20)
{
	frontDist_ = 5.f;
	if (playerInfo_ != nullptr) {
		Vector3 colPos;

		//回転情報から正面ベクトル(2D)を取得
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
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(playerInfo_->WT->rotation_.y),
		0,
		cosf(playerInfo_->WT->rotation_.y),
	};
	frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;

	//maxTime - 10の時間分プレイヤーを前に進める
	if (attackInfo_.nowTime < attackInfo_.maxTime - 10) {
		*playerInfo_->addVec_ += speed;
		attackCol_.at(0)->col_.center += *playerInfo_->addVec_;
	}
}


