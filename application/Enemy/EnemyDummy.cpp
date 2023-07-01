#include "EnemyDummy.h"

EnemyDummy::EnemyDummy() : IEnemy()
{
	time_ = 60.f;
}

void EnemyDummy::MoveUpdate()
{
	time_ -= 1.f;
	//テスト用一定時間でジャンプする敵
	if (time_ < 0.f) {
		time_ = 60.f;
		gravity_.SetGrabity({ 0,0.4f,0 });
	}
	//右へ移動する
	addVec_.x = 0.01f;
}
