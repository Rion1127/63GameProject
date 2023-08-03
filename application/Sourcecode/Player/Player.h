#pragma once
#include "mInput.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"
#include "IActor.h"
#include "PlayerHPGauge.h"
#include "Guard.h"
#include "DodgeRoll.h"
#include "Sword.h"

class Player final:
	public IActor
{
private:

	//�x�N�g��
	Vector3 move_;
	Vector3 frontVec_;
	Vector3 lockOnVec_;
	Vector2 moveVec2_;
	// --����-- //
	float inputAngle_;// -> ���͂���Ă�������̊p�x

	float moveSpeed_;

	bool isJump_;
	bool isAlive_;
	int jumpTime_;

	AttackManager attack_;
	Guard guard_;
	DodgeRoll dodgeRoll_;
	PlayerHPGauge hpGauge_;
	PlayerState state_;
	//���n�d������
	Timer landingTimer_;

	int32_t maxHealth_;
	int32_t health_;

	Sphere damageCol_;

	Sword sword_;
private:
	Controller* controller_ = nullptr;
public:
	Player();
	void PreUpdate();
	void PostUpdate();

	void ColPosUpdate();
private:
	//�ړ�
	void InputVecUpdate();
	//�d��
	void GravityUpdate();
	//�W�����v
	void JumpUpdate();
	//�X�e�[�^�X�X�V
	void StateUpdate();
public:
	void Draw();

	void DrawImGui();

	void DrawSprite();

	void FloorColision(Vector3 pos);
	void WallColision();

	bool GetIsCanMove();
	bool GetIsCanDodge();
	bool GetIsCanGuard();
	bool GetIsCanJump();
	bool GetIsCanAttack();

	void Damage(int32_t damage, Vector3 knockVec);
	void GuardHit(Vector3 knockVec);
	void Reset();
public:
	void SetPos(Vector3 pos) { obj_->GetTransform()->SetPosition(pos); }
	void SetRot(Vector3 rot) { obj_->GetTransform()->SetRotation(rot); }
	void Setscale(Vector3 scale) { obj_->GetTransform()->SetScale(scale); }
	void SetKnockVec(Vector3 vec) override{ knockVec_ = vec; }
	void SetState(PlayerState state) { state_ = state; }

	void AddaddVec(Vector3 pos) { addVec_ += pos; }
	void SetAddPos(Vector3 pos) { addVec_ = pos; }
	void SetLockOnEnemy(IEnemy* enemy) { attack_.SetLockOnEnemy(enemy); }
	void SetIsFloorCollision(bool flag) { isFloorCollision_ = flag; }
public:
	AttackManager* GetAttackManager() { return &attack_; }
	Vector3 GetFrontVec() { return frontVec_; }
	PlayerState GetState() { return state_; }
	Timer* GetDamegeCoolTime() { return &damageCoolTime_; }
	bool GetIsAlive() { return isAlive_; }
	Vector3 GetLockOnVec() { return lockOnVec_; }
	Guard* GetGuard() { return &guard_; }
	Sphere GetDamageCol() { return damageCol_; }
};

