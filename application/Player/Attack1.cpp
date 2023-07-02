#include "Attack1.h"

Attack1::Attack1() : IAttack(1, 25)
{
	frontDist_ = 5.f;
	Vector3 frontVec{};
	if (splayerInfo_ != nullptr) {
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_) {
			Vector3 colPos;
			//回転情報から正面ベクトル(2D)を取得
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;

			frontVec = {
				 lockOnPos.x - splayerInfo_->WT->position_.x,
				0,
				lockOnPos.z - splayerInfo_->WT->position_.z,
			};

			Vector3 frontDist = frontVec * frontDist_;
			frontDist.y = 0;

			colPos = splayerInfo_->WT->position_ + frontDist;
			colPos.y += 1;
			attackCol_.at(0)->col_.center = colPos;
		}
		else {
			Vector3 colPos;

			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(splayerInfo_->WT->rotation_.y),
				0,
				cosf(splayerInfo_->WT->rotation_.y),
			};

			Vector3 frontDist = frontVec * frontDist_;
			frontDist.y = 0;

			colPos = splayerInfo_->WT->position_ + frontDist;
			colPos.y += 1;
			attackCol_.at(0)->col_.center = colPos;
		}
	}

	attackVec_ = frontVec;
}

void Attack1::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_.normalize();
	Vector3 speed = attackVec_ * 0.1f;

	//maxTime - 10の時間分プレイヤーを前に進める
	if (attackInfo_.nowTime < attackInfo_.maxTime - 10) {
		*splayerInfo_->addVec_ += speed;
		attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + attackVec_ * 5.f;
		attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y * 2.f;
	}
}


