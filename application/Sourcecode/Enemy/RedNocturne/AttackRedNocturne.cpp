#include "AttackRedNocturne.h"
#include "ParticleManager.h"
#include "ParticleFire.h"

/**
 * @file AttackRedNocturne.cpp
 * @brief レッドノクターンの遠距離攻撃（炎を飛ばす）
 */

AttackRedNocturne::AttackRedNocturne(IActor* selfActor) :
	IBullet(selfActor,120, 5, 120)
{

}

AttackRedNocturne::~AttackRedNocturne()
{
	fireEmitter_->isActive = false;
}

void AttackRedNocturne::Init()
{
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		colPos = selfActor_->GetWorldTransform()->position_;
	}
	attackCol_->col_.center = colPos;
	attackCol_->col_.radius = 0.6f;
	attackCol_->damage = 10;
	//ノックバック力
	attackCol_->knockPower = { 0.1f,0.3f,0.1f };
	attackCol_->knockVecY = 0.5f;

	Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
	lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
	//ロックオンしている敵へのベクトルをとる
	Vector3 frontVec = lockOnPos - selfActor_->GetWorldTransform()->position_;
	frontVec = frontVec.normalize();
	attackVec_ = frontVec;
	attackCol_->colObj_.SetIsVisible(false);

	bulletSpeed_ = 0.3f;


	Timer timer;
	timer.SetLimitTime(1);
	timer.SetTime(timer.GetLimitTimer());

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->pos = attackCol_->col_.center;
	ParticleManager::GetInstance()->AddParticle("fireBall", fireEmitter_);
	CalculateRotToLockOnActor(CalculateFrontVec());
}

void AttackRedNocturne::MoveUpdate()
{
	//回転情報からプレイヤーへのベクトルを取得
	Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
	lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
	Vector3 frontVec = lockOnPos - attackCol_->col_.center;
	frontVec = frontVec.normalize();
	//足していくベクトルを徐々にプレイヤーの方向に変えていく
	MoveTo(frontVec, 0.01f, attackVec_);

	attackCol_->col_.center += attackVec_ * bulletSpeed_;

	fireEmitter_->pos = attackCol_->col_.center;

	fireEmitter_->popCoolTime_.AddTime(1);
	if (fireEmitter_->popCoolTime_.GetIsEnd()) {
		fireEmitter_->particle->Add();

		fireEmitter_->popCoolTime_.Reset();
	}

}