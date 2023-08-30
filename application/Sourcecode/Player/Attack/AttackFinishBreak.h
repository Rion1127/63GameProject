#pragma once
#include "IAttack.h"
class AttackFinishBreak :
    public IAttack
{
private:
	float colRadius_;
	int32_t splineTime_;
	int32_t hitNum_;
	//当たり判定が回転するスピード
	float rotateSpeed_;
public:
	AttackFinishBreak(IActor* selfActor);
private:
	void Init() override;
	void MoveUpdate() override;

	void SplineUpdate();
};

