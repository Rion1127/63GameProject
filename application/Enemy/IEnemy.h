#pragma once
#include "Object3d.h"
#include "IActor.h"
#include "Collision.h"
#include "Gravity.h"
class IEnemy : public IActor
{
protected:
	static IActor* splayer_;
	//ƒ‚ƒfƒ‹
	Gravity gravity_;
	Vector3 addVec_;

	bool isLockOn_;
public:
	static void SetPlayer(IActor* player) { splayer_ = player; }
	void SetIsLockOn(bool flag) { isLockOn_ = flag; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsLockOn() { return isLockOn_; }
protected:
	virtual void MoveUpdate() = 0;
};

