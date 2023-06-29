#include "FirstAttack.h"

void FirstAttack::MoveUpdate()
{
	attackCol_.at(0)->col_.center.z += 0.02f;
}

FirstAttack::FirstAttack() : IAttack(1,60)
{
	attackCol_.at(0)->col_.center.z = 0;
}
