#pragma once
#include "IAttack.h"

/**
 * @file AttackJumpShadow.h
 * @brief シャドウのジャンプ攻撃攻撃
 */

class AttackJumpShadow :
	public IAttack
{
private:
	float frontDist_;
	Vector3 PToEVec_;
public:
	AttackJumpShadow(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

