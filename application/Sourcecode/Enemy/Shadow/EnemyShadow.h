#pragma once
#include "IEnemy.h"
#include "Timer.h"
#include "Spline.h"

#include <unordered_map>

class EnemyShadow :
	public IEnemy
{
public:
	enum class State {
		Idle,
		Following,
		Wander,
		HideMove,
		Attack,
		JumpAttack,
		KnockBack,
		None
	};
private:
	State state_;
	TimerFloat actionTimer_;

	float followLength;
	float moveSpeed;
	int32_t randRange_;

	float endRot_;
	float nowRot_;

	Spline spline_;
	bool isWanderInit_;
	TimerFloat sinkTimer_;

	TimerFloat slimeTimer_;

	std::unordered_map<State, int32_t> priority_;
private:
	//デバッグ
	std::string stateName_;
public:
	EnemyShadow(Vector3 pos, Vector3 rot);
	void SetIsNock(bool flag) override;
	void SetState(State state);
	void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) override {};
	void Draw()override;
private:
	void MoveUpdate() override;
	void DrawSprite() override;

	void Idle();
	void Following();
	void Wander();
	void HideMove();
	void Attack();
	void JumpAttack();
	void KnockBack();

	void SortPriority();

	void WanderInit();

	void StateUpdate(State state);
};

