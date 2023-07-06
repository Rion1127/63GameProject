#include "Attack2.h"

Attack2::Attack2() : IAttack(1, 25, 10, 26)
{
}

void Attack2::Init()
{
	Vector3 frontVec{};
	Vector3 colPos{};
	Vector3 frontDist{};
	if (splayerInfo_ != nullptr) {
		frontDist_ = splayerInfo_->WT->scale_.x;
		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr) {
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
			//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
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
			//��]��񂩂琳�ʃx�N�g��(2D)���擾
			frontVec = {
				sinf(splayerInfo_->WT->rotation_.y),
				0,
				cosf(splayerInfo_->WT->rotation_.y),
			};
		}
		frontVec.normalize();
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = splayerInfo_->WT->position_ + frontDist;
		colPos.y += 1;
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 1.1f;
		attackCol_.at(0)->damage = 10;
		//�m�b�N�o�b�N��
		attackCol_.at(0)->knockPower = { 0.3f,0.5f,0.3f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}

	attackVec_ = frontVec;
}

void Attack2::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	attackVec_.normalize();
	
	Vector3 speed = attackVec_ * 0.06f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;

	*splayerInfo_->addVec_ += speed;
	Vector3 attackVec = attackVec_ * (splayerInfo_->WT->scale_.x * 2.f);
	attackCol_.at(0)->col_.center = splayerInfo_->WT->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += splayerInfo_->WT->scale_.y;
}


