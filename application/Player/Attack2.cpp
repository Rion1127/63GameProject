#include "Attack2.h"

Attack2::Attack2() : IAttack(1, 30)
{
	attackCol_.at(0)->col_.center.z = 0;
}

void Attack2::MoveUpdate()
{
	
	attackCol_.at(0)->col_.center.x += 0.15f;
}

void Attack2::PlayerMoveUpdate()
{
}
