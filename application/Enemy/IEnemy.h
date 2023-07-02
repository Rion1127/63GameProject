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

	bool isLockOn;
public:
	static void SetPlayer(IActor* player) { splayer_ = player; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
public:
	Gravity GetGravity() { return gravity_; }
	bool GetIsLockOn() { return isLockOn; }
protected:
	virtual void MoveUpdate() = 0;
};

