#include "SecondAttack.h"

SecondAttack::SecondAttack() : IAttack(1, 30)
{
	attackCol_.at(0)->col_.center.z = 0;
}

void SecondAttack::MoveUpdate()
{
	
	attackCol_.at(0)->col_.center.x += 0.15f;
}
