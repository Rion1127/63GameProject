#include "FirstAttack.h"

void FirstAttack::MoveUpdate()
{
	attackCol_.at(0)->col_.center.z += 0.02f;
	if (controller_->GetButtons(PAD_B))
	{
		attackCol_.at(0)->col_.center.z = 0;
	}
}

FirstAttack::FirstAttack() : IAttack(1)
{
	
}
