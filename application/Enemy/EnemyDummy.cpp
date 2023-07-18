#include "EnemyDummy.h"
#include "Texture.h"
#include "Player.h"

EnemyDummy::EnemyDummy(Vector3 pos) : IEnemy(EnemyType::Ground, true,100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("Player", true));
	//obj_->SetAmbient("cube", { 0,0,1.0f });
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

}

void EnemyDummy::MoveUpdate()
{
	if (health_ < 100) {
		health_++;
	}

	pos2D = GetScreenPos(*obj_->GetTransform(), *Camera::scurrent_);

	Vector3 PtoEVec =
		obj_->WT_.position_ - IEnemy::splayer_->GetWorldTransform()->position_;
	PtoEVec = -PtoEVec.normalize();
	PtoEVec.y = 0;
	
	Quaternion target;

	target = VecToDir(PtoEVec);

	obj_->WT_.SetRotType(RotType::Quaternion);
	obj_->WT_.SetQuaternion(target);
}

void EnemyDummy::DrawSprite()
{
	
}
