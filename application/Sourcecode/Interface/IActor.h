#pragma once
#include "IParticle.h"
#include "Object3d.h"
#include "Collision.h"
#include "Gravity.h"
#include "Timer.h"

enum class ActorType {
	Player,
	Enemy
};

class IActor
{
protected:
	ActorType type_;
	//モデル
	std::unique_ptr<Object3d> obj_;
	Sphere col_;
	Vector3 addVec_;
	Gravity gravity_;
	Vector3 knockVec_;
	Vector3 knockResist_;
	Timer damageCoolTime_;
	Timer freezeTimer_;
	bool isFloorCollision_ = false;
	float knockDecreaseValue = 0.05f;
	bool isCanMove_ = true;
protected:
	void ObjUpdate();
public:
	IActor(ActorType type);
	Sphere GetCol() { return col_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
	Vector3 GetAddVec() {
		return addVec_;
	}
	Gravity* GetGravity() { return &gravity_; }
public:
	void AddaddVec(Vector3 vec) { addVec_ += vec; }
	void SetaddVec(Vector3 vec) { addVec_ = vec; }
	void SetGravity(Vector3 vec) { gravity_.SetGrabity(vec); }
	virtual void SetKnockVec(Vector3 vec) { knockVec_ = knockVec_; }
	void SetIsCanMove(bool flag) { isCanMove_ = flag; }
	void SetFreezeTime(uint32_t time) { freezeTimer_.SetLimitTime(time); freezeTimer_.Reset(); }
};

