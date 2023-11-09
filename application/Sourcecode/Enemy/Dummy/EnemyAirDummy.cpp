#include "EnemyAirDummy.h"

/**
 * @file EnemyAirDummy.cpp
 * @brief デバッグ用の空中の敵のクラス
 */

EnemyAirDummy::EnemyAirDummy(const Vector3& pos) : IEnemy(EnemyType::Air, false, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	obj_->SetShadowAtten(Vector3(0.1f, 0.22f, 0));
	obj_->SetShadowFactorAngle(Vector2(0.1f, 0.5f));

	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();
}

void EnemyAirDummy::MoveUpdate()
{
	if (health_ < 100) {
		health_++;
	}

	obj_->GetModel()->DrawShadowInfo("AirDummy");
}

void EnemyAirDummy::DrawSprite()
{
}
