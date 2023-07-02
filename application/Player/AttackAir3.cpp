#include "AttackAir3.h"

AttackAir3::AttackAir3() : IAttack(1, 30)
{
	Vector3 frontVec{};
	Vector3 colPos{};
	Vector3 frontDist{};
	if (splayerInfo_ != nullptr) {
		frontDist_ = splayerInfo_->WT->scale_.x;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_) {
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
			//ロックオンしている敵へのベクトルをとる
			frontVec = {
				 lockOnPos.x - splayerInfo_->WT->position_.x,
				0,
				lockOnPos.z - splayerInfo_->WT->position_.z,
			};
			frontVec.normalize();
			Vector2 frontVec2 = {
				frontVec.x,
				frontVec.z
			};
			float rotY = Radian(Vec2Angle(frontVec2));
			*splayerInfo_->rot_ = { 0,rotY ,0 };
		}
		else {
			//回転情報から正面ベクトル(2D)を取得
			frontVec = {
				sinf(splayerInfo_->WT->rotation_.y),
				0,
				cosf(splayerInfo_->WT->rotation_.y),
			};
		}
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = splayerInfo_->WT->position_ + frontDist;
		colPos.y += 1;
		splayerInfo_->gravity->SetGrabity({ 0,0,0 });
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 1.f;
	}

	attackVec_ = frontVec;
}

void AttackAir3::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	Vector3 frontVec = {
		sinf(splayerInfo_->WT->rotation_.y),
		0,
		cosf(splayerInfo_->WT->rotation_.y),
	};
	frontVec.normalize();
	Vector3 speed = frontVec * 0.15f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;


	*splayerInfo_->addVec_ += speed;
	attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + frontVec * 5.f;
	attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y;

}
