#pragma once
#include "IBullet.h"

/**
 * @file MagicFire.h
 * @brief プレイヤーの攻撃魔法
 */

class MagicFire :
	public IBullet
{
private:
	float frontDist_;
	float bulletSpeed_;
	std::shared_ptr<ContinuousEmitter> fireEmitter_;
	Timer freezeTimer_;
	Vector3 RecoilVec_;
public:
	~MagicFire();
	MagicFire(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

