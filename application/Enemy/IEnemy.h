#pragma once
#include "Object3d.h"
#include "IActor.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"
class IEnemy : public IActor
{
protected:
	static IActor* splayer_;
	//ƒ‚ƒfƒ‹
	Gravity gravity_;
	Vector3 addVec_;
	Vector3 knockVec_;

	bool isLockOn_;
	Timer damegeCoolTime_;
	//‘Ì—Í
	float health_;
public:
	static void SetPlayer(IActor* player) { splayer_ = player; }
	void SetIsLockOn(bool flag) { isLockOn_ = flag; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
	void HitPlayerAttack(Vector3 knockVec,float damageValue,int32_t cooltime);
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsLockOn() { return isLockOn_; }
	Timer GetDamageCoolTime() { return damegeCoolTime_; }
protected:
	virtual void MoveUpdate() = 0;
};

