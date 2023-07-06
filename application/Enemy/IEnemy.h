#pragma once
#include "Object3d.h"
#include "IActor.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"

enum class EnemyType {
	Ground,	
	Air
};

class IEnemy : public IActor
{
protected:
	static IActor* splayer_;

	EnemyType enemyType_;
	//モデル
	Gravity gravity_;
	Vector3 addVec_;
	Vector3 knockVec_;
	Vector3 knockResist_;

	bool isLockOn_;
	bool isGravityImpact_;	//重力を有効にする
	bool isDead_;
	Timer damegeCoolTime_;
	//体力
	size_t health_;
	size_t maxHealth_;
public:
	IEnemy(EnemyType type,bool isGravityImpact, size_t health);
	virtual ~IEnemy() {};
	static void SetPlayer(IActor* player) { splayer_ = player; }
	void SetIsLockOn(bool flag) { isLockOn_ = flag; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
	void HitPlayerAttack(Vector3 knockVec, size_t damageValue,int32_t cooltime);
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsLockOn() { return isLockOn_; }
	bool GetIsDead() { return isDead_; }
	Timer GetDamageCoolTime() { return damegeCoolTime_; }
	Vector3 GetKnockResist() { return knockResist_; }
	size_t GetHealth() { return health_; }
	size_t GetMaxHealth() { return maxHealth_; }
protected:
	virtual void MoveUpdate() = 0;
};

