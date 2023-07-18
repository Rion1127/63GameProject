#pragma once
#include "Object3d.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"
class IActor
{
protected:
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> obj_;
	Sphere col_;
	Vector3 addVec_;
	Gravity gravity_;
	Vector3 knockVec_;
	Vector3 knockResist_;
	Timer damageCoolTime_;
public:
	Sphere GetCol() { return col_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
	Vector3* GetAddVec() { return &addVec_; }
	Gravity* GetGravity() { return &gravity_; }
	void AddVec(Vector3 vec) { addVec_ += vec; }
	virtual void SetKnockVec(Vector3 vec) { knockVec_ = knockVec_; }
};

