#include "AttackAir3.h"

AttackAir3::AttackAir3() : IAttack(1, 30, 10, 31)
{
}

void AttackAir3::Init()
{
	Vector3 frontVec{};
	Vector3 colPos{};
	Vector3 frontDist{};
	if (splayerInfo_ != nullptr) {
		frontDist_ = 5.f;
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
			//ロックオンしている敵へのベクトルをとる
			frontVec = {
				 lockOnPos.x - splayerInfo_->WT->position_.x,
				0,
				lockOnPos.z - splayerInfo_->WT->position_.z,
			};
			frontVec.normalize();
			//敵へのベクトルから角度を計算する
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
		splayerInfo_->gravity->SetGrabity({ 0,0.12f,0 });
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 1.f;
		attackCol_.at(0)->damage = 20;
		//ノックバック力
		attackCol_.at(0)->knockPower = { 1.f,1.f,1.f };
		attackCol_.at(0)->knockVecY = 0.8f;
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
	Vector3 attackVec = attackVec_ * (splayerInfo_->WT->scale_.x * 2.f);
	attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y;
}
