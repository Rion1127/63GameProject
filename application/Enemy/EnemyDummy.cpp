#include "EnemyDummy.h"

EnemyDummy::EnemyDummy(Vector3 pos) : IEnemy(EnemyType::Ground, true)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	isLockOn_ = false;
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damegeCoolTime_.SetLimitTime(30);
	ColPosUpdate();
}

void EnemyDummy::MoveUpdate()
{
	//time_ -= 1.f;
	////テスト用一定時間でジャンプする敵
	//if (time_ < 0.f) {
	//	time_ = 60.f;
	//	gravity_.SetGrabity({ 0,0.4f,0 });
	//}
	////右へ移動する
	//addVec_.x = 0.01f;
}
