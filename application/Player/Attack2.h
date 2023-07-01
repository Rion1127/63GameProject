#pragma once
#include "IAttack.h"
class Attack2 :
    public IAttack
{
public:
	Attack2();
private:
	void MoveUpdate() override;
	void PlayerMoveUpdate() override;
};

