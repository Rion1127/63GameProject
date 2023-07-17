#pragma once
#include "IEnemy.h"
#include "Timer.h"

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
	Timer actionTimer_;
	
	float followLength;
	float moveSpeed;
	int32_t randRange_;

	Vector3 EtoPVec_;

	float endRot_;
	float nowRot_;

	
private:
	//デバッグ
	std::string stateName_;
public:
	EnemyShadow(Vector3 pos);
	void SetIsNock(bool flag) override;
	void SetState(State state);

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

	void PriorityUpdate();
	void UpdateVector();
};

