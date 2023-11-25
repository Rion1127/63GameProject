#include "IEnemy.h"
#include "Player.h"

/**
 * @file IEnemy.cpp
 * @brief インターフェースクラス
 * @brief 敵を作る際に継承するクラス
 */

Player* IEnemy::splayer_ = nullptr;

IEnemy::IEnemy(EnemyType type, bool isGravityImpact, int32_t health)
	: IActor(ActorType::Enemy)
{
	enemyType_ = type;
	isGravityImpact_ = isGravityImpact;
	health_ = health;
	maxHealth_ = health;
	isDead_ = false;
	isSoftLockOn_ = false;
	isHardLockOn_ = false;
	isKnock_ = false;
	isBulletShot_ = false;

	gravity_.SetAddValue({ 0,-0.01f,0 });
}

void IEnemy::PreUpdate()
{
	if (GetIsHitStopping())return;
	addVec_ = { 0,0,0 };
	if (isGravityImpact_ == true)
	{
		gravity_.Update();
	}
	damageCoolTime_.AddTime(1);

	if (health_ <= 0)
	{
		health_ = 0;
		isDead_ = true;
	}
	ColPosUpdate();

	MoveUpdate();

	Vector3 displayPos = {
		obj_->GetPos().x,
		obj_->GetPos().y + col_.radius,
		obj_->GetPos().z,
	};

	displayObj_->SetPos(displayPos);
	displayObj_->SetRot(obj_->GetRot());
	displayObj_->SetScale(obj_->GetScale());
	displayObj_->WT_.SetQuaternion(obj_->WT_.quaternion_);
	displayObj_->Update();
}

void IEnemy::PostUpdate()
{
	ObjUpdate();
}

void IEnemy::Draw()
{
	displayObj_->Draw();
}

void IEnemy::FloorColision(const Vector3& pos)
{
	FloorCollisionDerived();
	isFloorCollision_ = true;
	gravity_.SetGrabity({ 0,0.f,0 });
	obj_->WT_.SetPosition(pos);
}

void IEnemy::ColPosUpdate()
{

	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + col_.radius,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);

	//col_.radius = obj_->GetTransform()->scale_.y;
}

void IEnemy::Damage(const Vector3& knockVec, int32_t damageValue, float cooltime)
{
	if (damageCoolTime_.GetIsEnd())
	{
		knockVec_ = knockVec;
		damageCoolTime_.SetLimitTime(cooltime);
		damageCoolTime_.Reset();
		health_ -= damageValue;
	}
	DamageUpdate();
}

void IEnemy::UpdateEtoPVec()
{
	//プレイヤーへのベクトル
	Vector3& pos = splayer_->GetWorldTransform()->position_;
	EtoPVec_ = pos - obj_->WT_.position_;
	EtoPVec_.y = 0;
}
