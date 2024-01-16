#include "EnemyAirDummy.h"

/**
 * @file EnemyAirDummy.cpp
 * @brief デバッグ用の空中の敵のクラス
 */

EnemyAirDummy::EnemyAirDummy(const Vector3& pos) : IEnemy(EnemyType::Air, false, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true,false));
	obj_->SetShadowAtten(Vector3(0.1f, 0.22f, 0));
	obj_->SetShadowFactorAngle(Vector2(0.1f, 0.5f));

	displayObj_ = std::move(std::make_unique<Object3d>());
	displayObj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	displayObj_->SetShadowOffsetPos(Vector3(0, -1, 0));
	displayObj_->SetShadowAtten(Vector3(0.1f, 0.22f, 0));
	displayObj_->SetShadowFactorAngle(Vector2(0.1f, 0.5f));

	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	displayObj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();
}

void EnemyAirDummy::MoveUpdate()
{
	if (health_ < maxHealth_) {
		health_++;
	}
}

void EnemyAirDummy::DrawSprite()
{
}
