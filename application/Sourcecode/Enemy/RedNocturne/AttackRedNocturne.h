#pragma once
#include "IBullet.h"
#include "IParticle.h"

/**
 * @file AttackRedNocturne.h
 * @brief レッドノクターンの遠距離攻撃（炎を飛ばす）
 */

class AttackRedNocturne final :
	public IBullet
{
private:
	float frontDist_;
	float bulletSpeed_;
	std::shared_ptr<ContinuousEmitter> fireEmitter_;
public:
	AttackRedNocturne(IActor* selfActor);
	~AttackRedNocturne();
private:
	void Init() override;
	void MoveUpdate() override;
};

