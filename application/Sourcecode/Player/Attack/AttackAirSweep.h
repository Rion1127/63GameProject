#pragma once
#include "IAttack.h"
#include "Timer.h"
class AttackAirSweep :
    public IAttack
{
private:
	float frontDist_;
	//�����蔻�肪��]����X�s�[�h
	float rotateSpeed_;
	float colRadius_;
public:
	AttackAirSweep(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

};

