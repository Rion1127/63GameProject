#include "IEnemy.h"

void IEnemy::Update()
{
	MoveUpdate();
}

void IEnemy::Draw()
{
	model_->Draw();
}
