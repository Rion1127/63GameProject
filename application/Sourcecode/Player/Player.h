#pragma once
#include "mInput.h"
#include "Vector3.h"
#include <memory>
#include "Texture.h"
#include "PlayerInfo.h"
#include "Gravity.h"
#include "AttackManager.h"
#include "IActor.h"
#include "PlayerGauge.h"
#include "Guard.h"
#include "DodgeRoll.h"
#include "Sword.h"
#include "PlayerCommand.h"

#include "StateMachine.h"

/**
 * @file Player.h
 * @brief プレイヤーの挙動全てを処理している
 */

class Player final :
	public IActor, public StateMachine<PlayerState>
{
private:
	//ベクトル
	Vector3 normalGravity_;
	Vector3 cameraToPlayerVec_;	//カメラ→プレイヤーのベクトル
	Vector3 playerFrontVec_;	//プレイヤーが向いているベクトル
	Vector3 lockOnVec_;			//ロックオンしている敵へのベクトル
	Vector2 inputVec_;			//入力している敵へのベクトル
	Vector2 moveVec_;			//1フレームの移動ベクトル
	float goalinputAngle_;		//入力したY軸の角度
	float nowAngle_;			//現在プレイヤーが向いている角度
	float walklimitValue_;		//歩きと走るが切り替わる値
	float dashSpeed_;			//移動スピード
	float walkSpeed_;			//移動スピード
	float slerpSpeed_;			//姿勢線形補完スピード
	float rotangle_;			//姿勢線形補完スピード
	bool isCanJump_;			//ジャンプ可能フラグ
	bool isAlive_;				//生存フラグ
	bool isCanInput_;			//入力可能フラグ
	bool isDash_;			//入力可能フラグ
	float maxjumptimer;			//ジャンプ可能なフレーム数
	float jumpTime_;			//ジャンプしたフレーム数
	float jumpSpeed_;			//ジャンプスピード
	float dodgeRollSpeed_;			//ドッジロールの回転スピード
	float jumpRollSpeed_;			//ジャンプの回転スピード
	TimerFloat shakeTimer_;			//歩いているプレイヤーが横に揺れる動きを管理するタイマー
	TimerFloat dashParticleTimer_;	//歩いているプレイヤーのパーティクル生成を管理するタイマー
	PlayerCommand command_;			//コマンドの動きを管理する
	GuardClass guard_;				//防御を管理する
	DodgeRoll dodgeRoll_;			//ドッジロールを管理する
	PlayerHPGauge hpGaugeUI_;		//HPゲージを管理する
	PlayerState state_;				//プレイヤーのステートを管理する
	int32_t maxHealth_;			//最大HP
	int32_t health_;			//現在HP
	uint32_t landingTimer_;
	Sphere damageCol_;			//攻撃を受ける当たり判定
	Sword sword_;				//剣オブジェ
	Quaternion playerQuaternion_;	//X,Y,Z軸の合成
private:
	int32_t particleAddNum_;
	float particleTimer_;
	float particleScale_;
public:
	Player();
	void PreColUpdate();
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
	void PlayerRotUpdate();
private:
	//重力
	void GravityUpdate();
	//ステータス更新
	void StateUpdate();
	
public:
	void Draw();

	void DrawImGui();

	void DrawSprite();

	void FloorColision(const Vector3& pos);
	void WallColision();

	bool GetIsCanMove();
	bool GetIsCanDodge();
	bool GetIsCanGuard();
	bool GetIsCanJump();
	bool GetIsCanAttack();
	bool GetIsMove();

	void Damage(int32_t damage, const Vector3& knockVec);
	void GuardHit(const Vector3& knockVec);
	void Reset();
public:
	//セッター
	void SetPos(const Vector3& pos) { obj_->GetTransform()->SetPosition(pos); }
	void SetRot(const Vector3& rot) { obj_->GetTransform()->SetRotation(rot); }
	void Setscale(const Vector3& scale) { obj_->GetTransform()->SetScale(scale); }
	void SetKnockVec(const Vector3& vec) override { knockVec_ = vec; }
	void SetState(PlayerState state) { state_ = state; }
	void SetIsCanInput(bool flag) { isCanInput_ = flag; }
	void SetIsDash(bool flag) { isDash_ = flag; }


	void AddaddVec(const Vector3& pos) { addVec_ += pos; }
	void SetAddPos(const Vector3& pos) { addVec_ = pos; }
	void SetLockOnEnemy(IEnemy* enemy) { command_.SetLockOnEnemy(enemy); }
	

public:
	AttackManager* GetAttackManager() { return command_.GetAttackManager(); }
	Vector3 GetcameraToPlayerVec() { return cameraToPlayerVec_; }
	Vector3 GetPlayerFrontVec() { return playerFrontVec_; }
	PlayerState GetState() { return state_; }
	TimerFloat* GetDamegeCoolTime() { return &damageCoolTime_; }
	Timer* GetFreezeTime() { return &freezeTimer_; }
	bool GetIsAlive() { return isAlive_; }
	bool GetIsCanInput() { return isCanInput_; }
	bool GetIsFloorCollision() { return isFloorCollision_; }
	bool GetIsDash() { return isDash_; }

	Vector3 GetLockOnVec() { return lockOnVec_; }
	GuardClass* GetGuard() { return &guard_; }
	Sphere GetDamageCol() { return damageCol_; }
	Vector2 GetInputVec() { return inputVec_; }
};

