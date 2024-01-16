#pragma once
#include "BaseAttack.h"
#include "IAttack.h"
#include "IEnemy.h"
#include "PlayerInfo.h"
#include <vector>
#include <map>
#include "AttackDataPools.h"
#include "mInput.h"

/**
 * @file AttackManager.h
 * @brief プレイヤーの基本攻撃を管理するクラス
 */

class Player;

enum ComboNum {
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR
};

class AttackManager
{
private:
	static Player* player_;
	IEnemy* lockOnEnemy_;
	AttackDataPool datapool_;
	std::unique_ptr<BaseAttack> nowAttack_;	//現在処理している攻撃
	std::unique_ptr<BaseAttack> nextAttack_;	//次に処理する攻撃
	const int32_t MAX_COMBO = 3;
	int32_t comboNum;
	bool isAttacking;
	bool isHardLock_;	//false 近い敵をロックオン, true ロックオンを固定
	float PtoELength_;
	bool isNextAttack_;
	bool isNextSpecialAttack_;
	const float distLengthLimit_ = 4.f;
	const float distYaxisLimit_ = 1.5f;
	float soundpicthUp_;
	float soundpicthDown_;
public:
	AttackManager();

	void Attack();
	void Update();

	void DrawDebug();
private:
	void CalculatePtoELength();
	void FirstAttackUpdate();	//1コンボ目更新
	void NextComboUpdate();		//2コンボ目以降更新
	void SwitchAttack();

	bool GetIsNextAttack();
public:
	bool GetIsAttacking() { return isAttacking; }
public:
	void SetLockOnEnemy(IEnemy* enemy) { lockOnEnemy_ = enemy; }
public:
	BaseAttack* GetNowAttack() { return nowAttack_.get(); }
	IEnemy* GetLockOnEnemy() { return lockOnEnemy_; }
	int32_t GetComboNum() { return comboNum; }
	static void SetPlayer(Player* player) { player_ = player; }
};

