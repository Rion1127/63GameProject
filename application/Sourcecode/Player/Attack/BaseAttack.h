#pragma once
#include "AttackEditor.h"
#include "Spline.h"
#include "IActor.h"
#include "Timer.h"
#include "Collision.h"
#include <vector>

/**
 * @file BaseAttack.h
 * @brief プレイヤーの基本攻撃の動作をするクラス
 */

class BaseAttack
{
public:
	struct AttackInput {
		AttackType type_;		//通常攻撃かフィニッシュ技か
		std::vector<AttackEditor::AttackInfo> attackinfo;	//攻撃の詳細
	};
private:
	IActor* selfActor_;
	IActor* lockOnActor_;
	float damageCoolTime_;		//無敵時間
	AttackInput attackinput_;	//攻撃の詳細
	TimerFloat attackAllTime_;		//攻撃に掛かるフレーム
	Spline spline_;
	uint32_t index_;

	bool isAttaking_;	//攻撃中フラグ

	Vector3 swordPos_;	//剣の座標

	Sphere col_;

	std::unique_ptr<Object3d> obj_;
public:
	BaseAttack(const AttackInput& input, IActor* selfActor, IActor* lockOnActor);
	//初期化
	void SetNextAttack();
	//更新
	void Update();
	//スプライン曲線・当たり判定追加
	void Draw();
private:
	Vector3 CalculateFrontVec();
	void CalculateRotToLockOnActor();
public:
	//ゲッター
	bool GetIsAttaking() { return isAttaking_; }
	Vector3 GetSwordPos() { return swordPos_; }
	Sphere GetCol() { return col_; }
	float GetDamageCoolTime() { return damageCoolTime_; }
	int32_t GetDamage() { return attackinput_.attackinfo[index_].damage; }
	AttackType GetAttackType() { return attackinput_.type_; }
	Vector3 GetKnockVec() { return attackinput_.attackinfo[index_].knockVec; }
};

