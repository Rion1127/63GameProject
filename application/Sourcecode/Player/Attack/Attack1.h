#pragma once
#include "IAttack.h"
#include "IActor.h"

class Attack1 final :
	public IAttack
{
private:
	float attackSpeed_;
public:
	Attack1(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

private:
	void SplineInit();
};

