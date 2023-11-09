#pragma once
#include "IAttack.h"

/**
 * @file AttackShadow.h
 * @brief シャドウの通常攻撃（前方向）
 */

class AttackShadow final :
	public IAttack
{
private:
	float frontDist_;
public:
	AttackShadow(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

};

