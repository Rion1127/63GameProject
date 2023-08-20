#pragma once
#include "mInput.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"
#include "MagicManager.h"
#include "IActor.h"
#include "PlayerHPGauge.h"
#include "Guard.h"
#include "DodgeRoll.h"
#include "Sword.h"
#include "PlayerCommand.h"

#include "StateMachine.h"

class Player final:
	public IActor , public StateMachine<PlayerState>
{
private:
	//ベクトル
	Vector3 frontVec_;
	Vector3 lockOnVec_;
	Vector2 inputVec_;
	Vector2 moveVec_;
	// --入力-- //
	float inputAngle_;	//入力されている方向の角度
	float walklimitValue_;	//入力

	float moveSpeed_;

	bool isCanJump_;
	bool isAlive_;
	int jumpTime_;

	
	PlayerCommand command_;
	GuardClass guard_;
	DodgeRoll dodgeRoll_;
	PlayerHPGauge hpGauge_;
	PlayerState state_;

	int32_t maxHealth_;
	int32_t health_;

	Sphere damageCol_;

	Sword sword_;
public:
	Player();
	void PreUpdate();
	void PostUpdate();

	void ColPosUpdate();

	// ステートマシンを初期化する。
	void InitStateMachine() override;

	void DogeRoll();
	void DogeRollUpdate();
	void Guard();
	void GuardUpdate();
	//ジャンプ
	void Jump();
	void JumpUpdate();
	void Freeze(uint32_t time);
	void FreezeUpdate();
	//移動
	void InputVecUpdate();
private:
	//重力
	void GravityUpdate();
	//ステータス更新
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
	void SetLockOnEnemy(IEnemy* enemy) { command_.SetLockOnEnemy(enemy); }
	void SetIsFloorCollision(bool flag) { isFloorCollision_ = flag; }
public:
	AttackManager* GetAttackManager() { return command_.GetAttackManager(); }
	MagicManager* GetMagicManager() { return command_.GetMagicManager(); }
	Vector3 GetFrontVec() { return frontVec_; }
	PlayerState GetState() { return state_; }
	Timer* GetDamegeCoolTime() { return &damageCoolTime_; }
	Timer* GetFreezeTime() { return &freezeTimer_; }
	bool GetIsAlive() { return isAlive_; }
	Vector3 GetLockOnVec() { return lockOnVec_; }
	GuardClass* GetGuard() { return &guard_; }
	Sphere GetDamageCol() { return damageCol_; }
	Vector2 GetInputVec() { return inputVec_; }
};

