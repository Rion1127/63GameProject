#include "EnemyDummy.h"
#include "Texture.h"
#include "Player.h"
#include <imgui.h>

/**
 * @file EnemyDummy.cpp
 * @brief デバッグ用の地上の敵のクラス
 */

EnemyDummy::EnemyDummy(const Vector3& pos) : IEnemy(EnemyType::Ground, true, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("dummyEnemy", true,false));
	obj_->WT_.SetRotType(RotType::Quaternion);

	displayObj_ = std::move(std::make_unique<Object3d>());
	displayObj_->SetModel(Model::CreateOBJ_uniptr("dummyEnemy", true));
	displayObj_->WT_.SetRotType(RotType::Quaternion);
	displayObj_->SetShadowOffsetPos(Vector3(0,-1,0));
	knockResist_ = { 1,1,1 };
	baseGravity_ = Vector3(0, -0.007f, 0);

	obj_->GetTransform()->SetPosition(pos);
	displayObj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();
	gravity_.SetAddValue(baseGravity_);

	maxHealth_ = 100;
}

void EnemyDummy::MoveUpdate()
{
	if (health_ < maxHealth_) {
		health_++;
	}
	//向きを常にプレイヤーの方向へ向ける
	Vector3 PtoEVec =
		obj_->WT_.position_ - IEnemy::splayer_->GetWorldTransform()->position_;
	PtoEVec = -PtoEVec.normalize();
	PtoEVec.y = 0;
	Quaternion target;
	target = VecToDir(PtoEVec);
	obj_->WT_.SetQuaternion(target);
	col_.radius = obj_->GetTransform()->scale_.y;
}

void EnemyDummy::DrawSprite()
{

}
