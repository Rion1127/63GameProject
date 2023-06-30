#pragma once
#include "IAttack.h"
class SecondAttack :
    public IAttack
{
public:
	SecondAttack();
private:
	void MoveUpdate() override;
	void PlayerMoveUpdate() override;
};

