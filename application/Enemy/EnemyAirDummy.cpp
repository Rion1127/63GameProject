#include "EnemyAirDummy.h"

EnemyAirDummy::EnemyAirDummy(Vector3 pos) : IEnemy(EnemyType::Air, false, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	

	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();
}

void EnemyAirDummy::MoveUpdate()
{
}

void EnemyAirDummy::DrawSprite()
{
}
