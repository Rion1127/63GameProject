#include "AttackAir1.h"

AttackAir1::AttackAir1() : IAttack(1, 20)
{
	frontDist_ = 5.f;
	if (splayerInfo_ != nullptr) {
		Vector3 colPos;

		//回転情報から正面ベクトル(2D)を取得
		Vector3 frontVec = {
			sinf(splayerInfo_->WT->rotation_.y),
			0,
			cosf(splayerInfo_->WT->rotation_.y),
		};

		Vector3 frontDist = frontVec * frontDist_;
		frontDist.y = 0;

		colPos = splayerInfo_->WT->position_ + frontDist;
		colPos.y += 1;
		splayerInfo_->gravity->SetGrabity({ 0,0 ,0 });
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 1.f;
	}
}

void AttackAir1::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(splayerInfo_->WT->rotation_.y),
		0,
		cosf(splayerInfo_->WT->rotation_.y),
	};
	frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;

	
	*splayerInfo_->addVec_ += speed;
	attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + frontVec * 5.f;
	attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y;
}
