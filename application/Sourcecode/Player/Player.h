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
#include "PlayerGauge.h"
#include "Guard.h"
#include "DodgeRoll.h"
#include "Sword.h"
#include "PlayerCommand.h"

#include "StateMachine.h"

class Player final :
	public IActor, public StateMachine<PlayerState>
{
private:
	std::unique_ptr<Object3d> displayObj_;	//描画に使用しているオブジェ
	//ベクトル
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

	bool isCanJump_;			//ジャンプ可能フラグ
	bool isAlive_;				//生存フラグ
	bool isCanInput_;			//入力可能フラグ
	bool isDash_;			//入力可能フラグ
	float maxjumptimer;			//ジャンプ可能なフレーム数
	float jumpTime_;			//ジャンプしたフレーム数
	float jumpSpeed_;			//ジャンプスピード

	TimerFloat shakeTimer_;			//歩いているプレイヤーが横に揺れる動きを管理するタイマー
	TimerFloat dashParticleTimer_;	//歩いているプレイヤーのパーティクル生成を管理するタイマー

	PlayerCommand command_;			//コマンドの動きを管理する
	GuardClass guard_;				//防御を管理する
	DodgeRoll dodgeRoll_;			//ドッジロールを管理する
	PlayerHPGauge hpGaugeUI_;		//HPゲージを管理する
	PlayerMPGauge mpGaugeUI_;		//MPゲージを管理する
	PlayerState state_;				//プレイヤーのステートを管理する

	int32_t maxHealth_;			//最大HP
	int32_t health_;			//現在HP

	int32_t maxMP_;				//最大MP
	int32_t nowMP_;				//現在MP
	Timer mpChargeTime_;		//MPチャージ時間
	Timer mpChargeIntervalTimer_;	//MPを一定時間ごとに回復するタイマー
	bool isMPCharge_;			//MPチャージ中フラグ

	uint32_t landingTimer_;

	Sphere damageCol_;			//攻撃を受ける当たり判定

	Sword sword_;				//剣オブジェ

	Quaternion axisX_;				//プレイヤーX軸回転
	Quaternion axisY_;				//プレイヤーY軸回転
	Quaternion axisZ_;				//プレイヤーZ軸回転
	Quaternion playerQuaternion_;	//X,Y,Z軸の合成
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
	void MPCharge();
	void PlayerRotUpdate();
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


	void AddaddVec(const Vector3& pos) { addVec_ += pos; }
	void SetAddPos(const Vector3& pos) { addVec_ = pos; }
	void SetLockOnEnemy(IEnemy* enemy) { command_.SetLockOnEnemy(enemy); }
	void SetIsFloorCollision(bool flag) { isFloorCollision_ = flag; }

	void SubMP(int32_t costMp) { nowMP_ -= costMp; }
public:
	AttackManager* GetAttackManager() { return command_.GetAttackManager(); }
	MagicManager* GetMagicManager() { return command_.GetMagicManager(); }
	Vector3 GetcameraToPlayerVec() { return cameraToPlayerVec_; }
	Vector3 GetPlayerFrontVec() { return playerFrontVec_; }
	PlayerState GetState() { return state_; }
	Timer* GetDamegeCoolTime() { return &damageCoolTime_; }
	Timer* GetFreezeTime() { return &freezeTimer_; }
	bool GetIsAlive() { return isAlive_; }
	bool GetIsMPCharge() { return isMPCharge_; }
	bool GetIsCanInput() { return isCanInput_; }
	bool GetIsFloorCollision() { return isFloorCollision_; }

	Vector3 GetLockOnVec() { return lockOnVec_; }
	GuardClass* GetGuard() { return &guard_; }
	Sphere GetDamageCol() { return damageCol_; }
	Vector2 GetInputVec() { return inputVec_; }
};

