#pragma once
#include "IAttack.h"

class FirstAttack :
	public IAttack
{
public:
	FirstAttack();
private:
	void MoveUpdate() override;
};

