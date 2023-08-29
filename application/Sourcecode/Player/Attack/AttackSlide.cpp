#include "AttackSlide.h"

AttackSlide::AttackSlide(IActor* selfActor) :
	IAttack(selfActor,1,30,10,30)
{
}

void AttackSlide::Init()
{
	Vector3 frontVec = CalculateFrontVec();
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		
		//ロックオンしている敵がいるなら
		if (IAttack::lockOnActor_ != nullptr) {
			CalculateRotToLockOnActor(frontVec);
		}
		
		colPos = selfActor_->GetWorldTransform()->position_ + frontDist;
		colPos.y += 1;
	}
	attackVec_ = frontVec;
	
	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->col_.radius = 1.f;
	attackCol_.at(0)->damage = 10;
	//ノックバック力
	attackCol_.at(0)->knockPower = { 0.1f,0.1f,0.1f };
	attackCol_.at(0)->knockVecY = 0.5f;

	state_ = State::Slide;

	speed_ = 0.6f;
}

void AttackSlide::MoveUpdate()
{
	Vector3 moveVec{};
	Vector3 colPos{};
	if (state_ == State::Slide) {

		float rate = (float)attackInfo_.nowTime / (float)attackInfo_.maxTime;

		if (attackCol_.at(0)->isCollision_ || 
			rate >= 0.6f)
		{
			state_ = State::SpeedDown;
		}


	}
	else if (state_ == State::SpeedDown) {
		speed_ -= 0.05f;
		speed_ = Max(speed_, 0.f);
	}
	moveVec = CalculateFrontVec().normalize() * speed_;
	selfActor_->AddaddVec(moveVec);

	colPos = selfActor_->GetWorldTransform()->position_;
	colPos += CalculateFrontVec().normalize() * 2.5f;
	colPos.y += 0.7f;
	attackCol_.at(0)->col_.center = colPos;
	attackCol_.at(0)->damageCoolTime = attackInfo_.maxTime - attackInfo_.nowTime + 1;
}
