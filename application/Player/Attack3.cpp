#include "Attack3.h"

Attack3::Attack3() : IAttack(1, 30, 10, 31)
{
}

void Attack3::Init()
{
	Vector3 frontVec{};
	Vector3 colPos{};
	Vector3 frontDist{};
	if (splayerInfo_ != nullptr) {
		frontDist_ = splayerInfo_->WT->scale_.x;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
			//ロックオンしている敵へのベクトルをとる
			frontVec = {
				 lockOnPos.x - splayerInfo_->WT->position_.x,
				0,
				lockOnPos.z - splayerInfo_->WT->position_.z,
			};
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
		frontVec = frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = splayerInfo_->WT->position_ + frontDist;
		colPos.y += 1;
	}

	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->col_.radius = 1.2f;
	attackCol_.at(0)->damage = 20;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 1.f,1.f,1.f };
	attackCol_.at(0)->knockVecY = 0.8f;

	attackVec_ = frontVec;
}

void Attack3::MoveUpdate()
{
	//回転情報から正面ベクトル(2D)を取得
	attackVec_ = attackVec_.normalize();

	Vector3 speed = attackVec_ * 0.1f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;


	*splayerInfo_->addVec_ += speed;
	Vector3 attackVec = attackVec_ * (splayerInfo_->WT->scale_.x * 2.f);
	attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y;
}

