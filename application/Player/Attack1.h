#pragma once
#include "IAttack.h"
#include "IActor.h"

class Attack1 final :
	public IAttack
{
private:
	float frontDist_;
public:
	Attack1(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;
};

