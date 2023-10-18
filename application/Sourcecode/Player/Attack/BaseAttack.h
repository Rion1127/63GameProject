#pragma once
#include "AttackEditor.h"
#include "Spline.h"
#include "IActor.h"
#include "Timer.h"
#include <vector>

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
	float DamageCoolTime_;		//無敵時間
	AttackInput attackinput_;	//攻撃の詳細
	TimerFloat attackAllTime_;		//攻撃に掛かるフレーム
	Spline spline_;
	uint32_t index_;

	bool isAttaking_;	//攻撃中フラグ

	Vector3 swordPos_;	//剣の座標

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
};

