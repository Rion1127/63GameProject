#pragma once
#include "Object3d.h"
#include "AttackManager.h"
#include "IAttack.h"
class Sword
{
private:
	std::unique_ptr<Object3d> obj_;
	Object3d* playerObj_;
	Vector3 localPos_;

	Vector3 goalPos_;
	Vector3 nowPos_;

	AttackManager* attackManager_;
public:
	Sword();
	void Update();
	void Draw();
	
public:
	void SetAttackManager(AttackManager* attackManager) { attackManager_ = attackManager; }
	void SetPos(const Vector3& pos) { obj_->SetPos(pos); }
	void SetParent(Object3d* parent) { playerObj_ = parent; }
};

