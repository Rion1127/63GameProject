#include "AttackAir2.h"

AttackAir2::AttackAir2(IActor* selfActor, IActor* lockOnActor) :
	IAttack(selfActor, lockOnActor,1, 20, 10, 21)
{
}

void AttackAir2::Init()
{
	Vector3 frontVec{};
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		frontDist_ = selfActor_->GetWorldTransform()->scale_.x;
		//���b�N�I�����Ă���G������Ȃ�
		if (IAttack::lockOnActor_ != nullptr) {
			Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
			//���b�N�I�����Ă���G�ւ̃x�N�g�����Ƃ�
			frontVec = {
				 lockOnPos.x - selfActor_->GetWorldTransform()->position_.x,
				0,
				lockOnPos.z - selfActor_->GetWorldTransform()->position_.z,
			};
			frontVec = frontVec.normalize();
			Vector2 frontVec2 = {
				frontVec.x,
				frontVec.z
			};
			float rotY = Radian(Vec2Angle(frontVec2));
			selfActor_->GetWorldTransform()->rotation_ = { 0,rotY ,0 };
		}
		else {
			//��]��񂩂琳�ʃx�N�g��(2D)���擾
			frontVec = {
				sinf(selfActor_->GetWorldTransform()->rotation_.y),
				0,
				cosf(selfActor_->GetWorldTransform()->rotation_.y),
			};
		}
		frontDist = frontVec * frontDist_;
		frontDist.y = 0;
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
		selfActor_->GetGravity()->SetGrabity({0,0.1f,0});
		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 1.f;
		attackCol_.at(0)->damage = 10;
		//�m�b�N�o�b�N��
		attackCol_.at(0)->knockPower = { 0.3f,0.3f,0.3f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}

	attackVec_ = frontVec;
}

void AttackAir2::MoveUpdate()
{
	//��]��񂩂琳�ʃx�N�g��(2D)���擾
	Vector3 frontVec = {
		sinf(selfActor_->GetWorldTransform()->rotation_.y),
		0,
		cosf(selfActor_->GetWorldTransform()->rotation_.y),
	};
	frontVec = frontVec.normalize();
	Vector3 speed = frontVec * 0.1f;
	float timerate = 1.f - (float)attackInfo_.nowTime / attackInfo_.maxTime;
	speed *= timerate;


	selfActor_->AddVec(speed);
	Vector3 attackVec = attackVec_ * (selfActor_->GetWorldTransform()->scale_.x * 2.f);
	attackCol_.at(0)->col_.center = selfActor_->GetWorldTransform()->position_ + attackVec;
	attackCol_.at(0)->col_.center.y += selfActor_->GetWorldTransform()->scale_.y;
}
