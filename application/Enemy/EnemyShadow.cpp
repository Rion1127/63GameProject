#include "EnemyShadow.h"
#include "mInput.h"


EnemyShadow::EnemyShadow(Vector3 pos) : IEnemy(EnemyType::Ground, true, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	obj_->SetAmbient("cube", { 0,0,1.0f });
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damegeCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(240);
}

void EnemyShadow::MoveUpdate()
{
	if (health_ < 100)
	{
		health_ += 5;
	}
	//ステータスごとの動きを追加
	void (EnemyShadow:: *Action[]) () =
	{
		&EnemyShadow::Idle,
		&EnemyShadow::Move,
		&EnemyShadow::HideMove,
		&EnemyShadow::Attack,
		&EnemyShadow::JumpAttack
	};
	//実行
	(this->*Action[(int)state_])();

}

void EnemyShadow::DrawSprite()
{
	
}

void EnemyShadow::Idle()
{
	addVec_.x += 0.001f;
}

void EnemyShadow::Move()
{
	addVec_.x += 0.05f;
}

void EnemyShadow::HideMove()
{
	addVec_.x -= 0.05f;
}

void EnemyShadow::Attack()
{
	addVec_.z += 0.01f;
}

void EnemyShadow::JumpAttack()
{
	addVec_.z -= 0.001f;
}
