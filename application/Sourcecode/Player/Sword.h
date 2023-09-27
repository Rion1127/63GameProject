#pragma once
#include "Object3d.h"
#include "AttackManager.h"
#include "IAttack.h"
#include "Timer.h"
#include "SwordTrail.h"
class Sword
{
public:
	enum class SwordState {
		Idle,
		Attack,
		Guard,
		MagicFire,
		Debug
	};
private:
	std::unique_ptr<Object3d> obj_;
	Object3d* playerObj_;
	SwordState state_;
	Vector3 localPos_;
	//Idle注にふわふわ浮くようにするタイマー
	TimerFloat floatingTimer_;

	Vector3 goalPos_;
	Vector3 nowPos_;

	float rot_;

	AttackManager* attackManager_;

	std::vector<std::unique_ptr<Object3d>> tailObj_;
	SwordTrail::PosBuffer tailPos_;
	std::unique_ptr<SwordTrail> trail_;
public:
	Sword();
	void Update();
	void Draw();
private:
	void CalculateTrailPos();
public:
	void SetAttackManager(AttackManager* attackManager) { attackManager_ = attackManager; }
	void SetPos(const Vector3& pos) { obj_->SetPos(pos); }
	void SetParent(Object3d* parent) { playerObj_ = parent; }
	void SetState(SwordState state) {
		if (state_ != SwordState::Debug) {
			state_ = state;
		}
	}
};

