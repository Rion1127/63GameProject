#pragma once
#include "IAttack.h"

class Attack1 :
	public IAttack
{
private:
	float frontDist_;
public:
	Attack1();
private:
	void MoveUpdate() override;
	void PlayerMoveUpdate() override;
};

