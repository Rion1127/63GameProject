#include "EnemyDummy.h"

EnemyDummy::EnemyDummy() : IEnemy()
{
	time_ = 60.f;
}

void EnemyDummy::MoveUpdate()
{
	time_ -= 1.f;
	//�e�X�g�p��莞�ԂŃW�����v����G
	if (time_ < 0.f) {
		time_ = 60.f;
		gravity_.SetGrabity({ 0,0.4f,0 });
	}
	//�E�ֈړ�����
	addVec_.x = 0.01f;
}
