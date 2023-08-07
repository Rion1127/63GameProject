#pragma once
#include "IEnemy.h"
class EnemyRedNocturne :
    public IEnemy
{
public:
	enum class State {
		Idle,
		Wander,
		Attack,
		KnockBack,
		None
	};
private:
	State state_;
	Timer actionTimer_;

	float followLength;
	float moveSpeed;
	int32_t randRange_;

	Vector3 wanderEndPos_;
	Vector3 wanderStartPos_;
	Vector3 pos_;

	bool stateInit;

	std::unordered_map<State, int32_t> priority_;
public:
	EnemyRedNocturne(Vector3 pos);
	void SetIsNock(bool flag) override;
	void BulletShot(std::list<std::unique_ptr<IBullet>>* bullets) override;
	void Draw()override;
private:
	void MoveUpdate() override;
	void DrawSprite() override {};

	void Idle();
	void Wander();
	void Attack();
	void KnockBack();
};

