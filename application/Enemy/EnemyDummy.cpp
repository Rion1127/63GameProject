#include "EnemyDummy.h"
#include "Texture.h"

EnemyDummy::EnemyDummy(Vector3 pos) : IEnemy(EnemyType::Ground, true,100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damegeCoolTime_.SetLimitTime(30);
	ColPosUpdate();

}

void EnemyDummy::MoveUpdate()
{
	if (health_ < 100) {
		health_ += 5;
	}

	pos2D = GetScreenPos(*obj_->GetTransform(), *Camera::scurrent_);
}

void EnemyDummy::DrawSprite()
{
	
}
