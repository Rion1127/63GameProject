#pragma once
#include "Object3d.h"
#include "Collision.h"
#include "Gravity.h"
class IActor
{
protected:
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> obj_;
	Sphere col_;
	Vector3 addVec_;
	Gravity gravity_;
public:
	Sphere GetCol() { return col_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
	Vector3* GetAddVec() { return &addVec_; }
	Gravity* GetGravity() { return &gravity_; }
};

