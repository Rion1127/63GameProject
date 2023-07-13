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
	//ÉÇÉfÉã
	Gravity gravity_;
	Vector3 addVec_;
	Vector3 knockVec_;
	Vector3 knockResist_;

	bool isSoftLockOn_;
	bool isHardLockOn_;
	bool isGravityImpact_;	//èdóÕÇóLå¯Ç…Ç∑ÇÈ
	bool isDead_;
	Timer damegeCoolTime_;
	//ëÃóÕ
	int32_t health_;
	int32_t maxHealth_;
public:
	IEnemy(EnemyType type,bool isGravityImpact, int32_t health);
	virtual ~IEnemy() {};
	static void SetPlayer(IActor* player) { splayer_ = player; }
	void SetSoftIsLockOn(bool flag) { isSoftLockOn_ = flag; }
	void SetHardIsLockOn(bool flag) { isHardLockOn_ = flag; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
	void HitPlayerAttack(Vector3 knockVec, int32_t damageValue,int32_t cooltime);

	virtual void DrawSprite() = 0;
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsSoftLockOn() { return isSoftLockOn_; }
	bool GetIsHardLockOn() { return isHardLockOn_; }
	bool GetIsDead() { return isDead_; }
	Timer GetDamageCoolTime() { return damegeCoolTime_; }
	Vector3 GetKnockResist() { return knockResist_; }
	int32_t GetHealth() { return health_; }
	int32_t GetMaxHealth() { return maxHealth_; }
protected:
	virtual void MoveUpdate() = 0;
};

