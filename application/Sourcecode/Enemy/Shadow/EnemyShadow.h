#pragma once
#include "IEnemy.h"
#include "Timer.h"
#include "Spline.h"

#include <unordered_map>

/**
 * @file EnemyShadow.h
 * @brief 地上の敵（シャドウ）のクラス
 */

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
		Down,
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
	Quaternion EToPQuaternion_;
	Quaternion knockQuaternion_;

	std::unique_ptr<Object3d> handObj_;
	Timer attackTimer_;
private:
	//デバッグ
	std::string stateName_;
public:
	EnemyShadow(const Vector3& pos,const Vector3& rot);
	void SetIsNock(bool flag) override;
	void SetIsDown(bool flag) override;
	void SetState(State state);
	void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) override { bullets; };
	void Draw()override;
private:
	void MoveUpdate() override;
	void DrawSprite() override;
	void DamageUpdate() override;
	//地面に当たった時の処理
	void FloorCollisionDerived() override;

	void Idle();
	void Following();
	void Wander();
	void HideMove();
	void Down();
	void Attack();
	void JumpAttack();
	void KnockBack();

	void SortPriority();

	void WanderInit();

	void StateUpdate(State state);
};

