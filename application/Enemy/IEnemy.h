#pragma once
#include "Object3d.h"
#include "Player.h"
#include "Collision.h"
#include "Gravity.h"
class IEnemy
{
protected:
	static Player* splayer_;
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> obj_;
	Sphere col_;
	Gravity gravity_;
	Vector3 addVec_;
public:
	IEnemy();
	static void SetPlayer(Player* player) { splayer_ = player; }

	void PreUpdate();
	void PostUpdate();
	void Draw();
	void FloorColision();
	void ColPosUpdate();
public:
	Sphere GetCol() { return col_; }
	Gravity GetGravity() { return gravity_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
protected:
	virtual void MoveUpdate() = 0;
};

