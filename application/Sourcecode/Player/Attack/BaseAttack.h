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
	struct AttackData {
		AttackType type_;		//通常攻撃かフィニッシュ技か
		std::vector<AttackEditor::AttackInfo> attackinfo;	//攻撃の詳細
	};
private:
	IActor* selfActor_;
	IActor* lockOnActor_;
	float damageCoolTime_;		//無敵時間
	AttackData attackdata_;	//攻撃の詳細
	TimerFloat attackAllTime_;		//攻撃に掛かるフレーム
	Spline spline_;			//剣の座標曲線
	Spline colSpline_;		//当たり判定曲線（coltype_ = Separateの場合もに使用）
	uint32_t index_;
	uint32_t quaternionIndex_;
	int32_t damage_;
	bool isAttaking_;	//攻撃中フラグ
	bool isCameraShake_;//カメラを揺らすフラグ
	float nextAttackFrame_;
	float slerpSpeed_;
	float quaternionTimer_;
	float oneSwingTimer_;
	Quaternion resultQuaternion_;
	Vector3 swordPos_;	//剣の座標
	Vector3 colPos_;	//当たり判定の座標
	Sphere col_;
	std::unique_ptr<Object3d> obj_;
	std::shared_ptr<IEmitter> emitter_;
public:
	BaseAttack(const AttackData& input, IActor* selfActor, IActor* lockOnActor);
	~BaseAttack();
	//初期化
	void SetNextAttack();
	//更新
	void Update();
	//スプライン曲線・当たり判定追加
	void Draw();
private:
	Vector3 CalculateFrontVec();		//正面ベクトルを計算
	void CalculateRotToLockOnActor();	//ロックオンしている方向にプレイヤーを向ける
	void NormalSplinePosInit();			//通常の当たり当たり判定
	void SeparateSplinePosInit();		//当たり判定は分裂している
	void PlayerMove();	//攻撃中のプレイヤーの動き
	void ColUpdate();	//当たり判定更新
	void QuaternionUpdate();
	void DamageCoolTimerUpdate();
public:
	//ゲッター
	bool GetIsAttaking() { return isAttaking_; }
	Vector3 GetSwordPos() { return swordPos_; }
	Sphere GetCol() { return col_; }
	float GetDamageCoolTime() { return damageCoolTime_; }
	int32_t GetDamage() { return damage_; }
	AttackType GetAttackType() { return attackdata_.type_; }
	AttackType GetNowSwingAttackType() { return attackdata_.attackinfo[index_].attackType; }
	Vector3 GetKnockVec() { return attackdata_.attackinfo[index_].knockVec; }
	float GetKnockYVec() { return attackdata_.attackinfo[index_].knockYVec; }
	AttackEffectEditor::EffectInfo GetEffectInfo() { return attackdata_.attackinfo[index_].effectInfo; }
	bool GetIsCameraShake() { return isCameraShake_; }
public:
	void SetIsCameraShake(bool flag) { isCameraShake_ = flag; }
};

