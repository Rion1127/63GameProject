#pragma once
#include "IAttack.h"
class AttackAirAerialFinish :
    public IAttack
{
private:
	float frontDist_;
	int32_t index_;
	float damegeCooltime_;
public:
	AttackAirAerialFinish(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

	void FirstAttackInit();
	void SecondAttackInit();
	void ThirdAttackInit();
	void FourthAttackInit();
};

