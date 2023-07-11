#include "EnemyDummy.h"
#include "Texture.h"

EnemyDummy::EnemyDummy(Vector3 pos) : IEnemy(EnemyType::Ground, true,100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	isLockOn_ = false;
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damegeCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	sprite_.Ini("Lockon");
	sprite_.SetTexture(TextureManager::GetInstance()->GetTexture("uv"));
}

void EnemyDummy::MoveUpdate()
{
	if (health_ < 100) {
		health_ += 5;
	}

	pos2D = TransformToVec2(*obj_->GetTransform(), *Camera::scurrent_);
	sprite_.SetPos(pos2D);
	sprite_.SetScale({ 0.1f, 0.1f });
	sprite_.Update();
}

void EnemyDummy::DrawSprite()
{
	sprite_.Draw();
}
