#include "AttackRedNocturne.h"

AttackRedNocturne::AttackRedNocturne(IActor* selfActor) :
	IAttack(selfActor, 1, 25, 5, 26)
{
	
}

void AttackRedNocturne::Init()
{
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {

		colPos = selfActor_->GetWorldTransform()->position_;

		attackCol_.at(0)->col_.center = colPos;
		attackCol_.at(0)->col_.radius = 0.6f;
		attackCol_.at(0)->damage = 10;
		//ノックバック力
		attackCol_.at(0)->knockPower = { 0.1f,0.3f,0.1f };
		attackCol_.at(0)->knockVecY = 0.5f;
	}

	Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
	//ロックオンしている敵へのベクトルをとる
	Vector3 frontVec = lockOnPos - selfActor_->GetWorldTransform()->position_;
	frontVec = frontVec.normalize();
	attackVec_ = frontVec;
	attackCol_.at(0)->colObj_.SetIsVisible(true);

	bulletSpeed_ = 0.5f;
}

void AttackRedNocturne::MoveUpdate()
{
	CalculateRotToLockOnActor(CalculateFrontVec());
	//回転情報から正面ベクトル(2D)を取得
	Vector3& lockOnPos = IAttack::lockOnActor_->GetWorldTransform()->position_;
	Vector3 frontVec = lockOnPos - attackCol_.at(0)->col_.center;
	frontVec = frontVec.normalize();
	MoveTo(frontVec,0.02f, attackVec_);

	attackCol_.at(0)->col_.center += attackVec_ * bulletSpeed_;
}