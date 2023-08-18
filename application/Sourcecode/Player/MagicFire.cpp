#include "MagicFire.h"
#include "ParticleManager.h"
#include "ParticleFire.h"

MagicFire::~MagicFire()
{
	fireEmitter_->isActive = false;
}

MagicFire::MagicFire(IActor* selfActor) :
	IBullet(selfActor, 1, 100, 5, 10)
{

}

void MagicFire::Init()
{
	Vector3 colPos{};
	Vector3 frontDist{};
	if (selfActor_ != nullptr) {
		colPos = selfActor_->GetWorldTransform()->position_;
	}
	colPos.y += selfActor_->GetWorldTransform()->scale_.y;
	attackCol_->col_.center = colPos;
	attackCol_->col_.radius = 0.6f;
	attackCol_->damage = 10;
	//ノックバック力
	attackCol_->knockPower = { 0.1f,0.3f,0.1f };
	attackCol_->knockVecY = 0.5f;

	bulletSpeed_ = 0.8f;

	if (IBullet::lockOnActor_ != nullptr) {
		Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
		lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
		//ロックオンしている敵へのベクトルをとる
		Vector3 frontVec = lockOnPos - selfActor_->GetWorldTransform()->position_;
		frontVec.y = 0;
		frontVec = frontVec.normalize();
		attackVec_ = frontVec;
		attackCol_->colObj_.SetIsVisible(false);
	}
	else {
		attackVec_ = CalculateFrontVec() * bulletSpeed_;
	}
	freezeTimer_.SetLimitTime(40);
	selfActor_->SetFreezeTime(freezeTimer_.GetLimitTimer());


	Timer timer;
	timer.SetLimitTime(1);
	timer.SetTime(timer.GetLimitTimer());

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	fireEmitter_->particle = std::make_unique<ParticleFire>();
	fireEmitter_->addVec = { 0.2f,0.2f, 0.2f, };
	fireEmitter_->addNum = 6;
	fireEmitter_->isActive = true;
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->time = 20;
	fireEmitter_->pos = attackCol_->col_.center;
	fireEmitter_->scale = 1.f;
	fireEmitter_->particle->SetEmitter(fireEmitter_.get());
	ParticleManager::GetInstance()->AddParticle("fireBall", fireEmitter_);
}

void MagicFire::MoveUpdate()
{
	freezeTimer_.AddTime(1);
	if (freezeTimer_.GetIsEnd() == false) {
		selfActor_->SetGravity(Vector3(0, 0, 0));
	}
	if (IBullet::lockOnActor_ != nullptr) {
		CalculateRotToLockOnActor(CalculateFrontVec());
		//回転情報からプレイヤーへのベクトルを取得
		Vector3 lockOnPos = IBullet::lockOnActor_->GetWorldTransform()->position_;
		lockOnPos.y += IBullet::lockOnActor_->GetWorldTransform()->scale_.y;
		Vector3 frontVec = lockOnPos - attackCol_->col_.center;
		frontVec.y = 0;
		frontVec = frontVec.normalize();
		//足していくベクトルを徐々にプレイヤーの方向に変えていく
		MoveTo(frontVec, 0.015f, attackVec_);
	}

	attackCol_->col_.center += attackVec_ * bulletSpeed_;

	fireEmitter_->pos = attackCol_->col_.center;

	fireEmitter_->popCoolTime_.AddTime(1);
	if (fireEmitter_->popCoolTime_.GetIsEnd()) {
		fireEmitter_->particle->Add();

		fireEmitter_->popCoolTime_.Reset();
	}

}
