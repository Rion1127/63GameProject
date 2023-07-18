#pragma once
#include "mInput.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "PlayerAttack.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"
#include "IActor.h"
#include "PlayerHPGauge.h"

class Player final:
	public IActor
{
private:
	Vector3 scale_;

	bool isFloorCollision_ = false;
	//ベクトル
	Vector3 move_;
	Vector3 frontVec_;
	
	//移動制限
	Vector2 MaxMinX_;
	Vector2 MaxMinY_;

	// --入力-- //
	float inputAngle_;// -> 入力されている方向の角度

	float moveSpeed_;

	bool isJump_;
	bool isAlive_;
	int jumpTime_;

	AttackManager attack_;
	PlayerHPGauge hpGauge_;
	PlayerState state_;
	//着地硬直時間
	Timer landingTimer_;

	int32_t maxHealth_;
	int32_t health_;
private:
	Controller* controller_ = nullptr;
public:
	Player();
	void PreUpdate();
	void PostUpdate();

	void ColPosUpdate();
private:
	//移動
	void InputVecUpdate();
	//重力
	void GravityUpdate();
	//ジャンプ
	void JumpUpdate();
	//ステータス更新
	void StateUpdate();
public:
	void Draw();

	void DrawImGui();

	void DrawSprite();

	void floorColision();

	bool GetIsCanMove();

	void Damage(int32_t damage, Vector3 knockVec);
public:
	void SetPos(Vector3 pos) { obj_->GetTransform()->SetPosition(pos); }
	void SetRot(Vector3 rot) { obj_->GetTransform()->SetRotation(rot); }
	void Setscale(Vector3 scale) { obj_->GetTransform()->SetScale(scale); }
	void SetKnockVec(Vector3 vec) override{ knockVec_ = vec; }
	void SetState(PlayerState state) { state_ = state; }

	void AddPos(Vector3 pos) { addVec_ += pos; }
	void SetAddPos(Vector3 pos) { addVec_ = pos; }
	void SetLockOnEnemy(IEnemy* enemy) { attack_.SetLockOnEnemy(enemy); }
	void SetIsFloorCollision(bool flag) { isFloorCollision_ = flag; }
public:
	AttackManager* GetAttackManager() { return &attack_; }
	Vector3 GetFrontVec() { return frontVec_; }
	PlayerState GetState() { return state_; }
	Timer* GetDamegeCoolTime() { return &damageCoolTime_; }
};

