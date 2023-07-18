#include "IEnemy.h"
#include "Player.h"

Player* IEnemy::splayer_ = nullptr;

IEnemy::IEnemy(EnemyType type, bool isGravityImpact, int32_t health)
{
	enemyType_ = type;
	isGravityImpact_ = isGravityImpact;
	health_ = health;
	maxHealth_ = health;
	isDead_ = false;
	isSoftLockOn_ = false;
	isHardLockOn_ = false;
	isKnock_ = false;
}

void IEnemy::PreUpdate()
{
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
}

void IEnemy::PostUpdate()
{
	MoveUpdate();

	addVec_ += gravity_.GetGravityValue();

	addVec_ += knockVec_;
	//ノックバックのベクトルを0にしていく
	MoveTo({ 0,0,0 }, 0.05f, knockVec_);
	obj_->WT_.position_ += addVec_;

	ColPosUpdate();
}

void IEnemy::Draw()
{
	obj_->Draw();
}

void IEnemy::FloorColision()
{
	gravity_.SetGrabity({ 0,0,0 });
}

void IEnemy::ColPosUpdate()
{
	obj_->Update();

	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + obj_->WT_.scale_.y,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);

	col_.radius = obj_->GetTransform()->scale_.x;
}

void IEnemy::HitPlayerAttack(Vector3 knockVec, int32_t damageValue, int32_t cooltime)
{
	if (damageCoolTime_.GetIsEnd())
	{
		knockVec_ = knockVec;
		damageCoolTime_.SetLimitTime(cooltime);
		damageCoolTime_.Reset();
		health_ -= damageValue;
	}

}
