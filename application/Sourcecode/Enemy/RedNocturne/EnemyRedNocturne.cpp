#include "EnemyRedNocturne.h"

EnemyRedNocturne::EnemyRedNocturne(Vector3 pos) :
	IEnemy(EnemyType::Air, false, 80)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("airEnemy", true));
	//obj_->SetAmbient("player", { 0,0,1.0f });
	knockResist_ = { 1,0,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(400);
}

void EnemyRedNocturne::SetIsNock(bool flag)
{
}

void EnemyRedNocturne::Draw()
{
	obj_->Draw();
}

void EnemyRedNocturne::MoveUpdate()
{
}
