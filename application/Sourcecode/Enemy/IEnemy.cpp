#include "IEnemy.h"
#include "Player.h"

Player* IEnemy::splayer_ = nullptr;

IEnemy::IEnemy(EnemyType type, bool isGravityImpact, int32_t health)
	: IActor()
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
}

void IEnemy::PreUpdate()
{
	addVec_ = { 0,0,0 };
	if (isGravityImpact_ == true)
	{
		gravity_.SetAddValue({ 0,-0.01f,0 });
		gravity_.Update();
	}
	damageCoolTime_.AddTime(1);

	if (health_ <= 0)
	{
		health_ = 0;
		isDead_ = true;
	}
	ColPosUpdate();

	
}

void IEnemy::PostUpdate()
{
	MoveUpdate();
	ObjUpdate();
}

void IEnemy::Draw()
{
	obj_->Draw();
}

void IEnemy::FloorColision(Vector3 pos)
{
	//addVec_ = { 0,0,0 };
	gravity_.SetGrabity({ 0,0.f,0 });
	obj_->WT_.SetPosition(pos);
}

void IEnemy::ColPosUpdate()
{

	Vector3 colPos = {
		obj_->GetTransform()->position_.x,
		obj_->GetTransform()->position_.y + obj_->WT_.scale_.y,
		obj_->GetTransform()->position_.z,
	};

	col_.SetPos(colPos);

	col_.radius = obj_->GetTransform()->scale_.x;
}

void IEnemy::Damage(Vector3 knockVec, int32_t damageValue, int32_t cooltime)
{
	if (damageCoolTime_.GetIsEnd())
	{
		knockVec_ = knockVec;
		damageCoolTime_.SetLimitTime(cooltime);
		damageCoolTime_.Reset();
		health_ -= damageValue;
	}

}

void IEnemy::UpdateEtoPVec()
{
	//プレイヤーへのベクトル
	Vector3& pos = splayer_->GetWorldTransform()->position_;
	EtoPVec_ = pos - obj_->WT_.position_;
	EtoPVec_.y = 0;
}
