#include "EnemyShadow.h"
#include "mInput.h"
#include "Player.h"
#include <imgui.h>

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

	followLength = 5.f;
	moveSpeed = 0.1f;
}

void EnemyShadow::SetIsNock(bool flag)
{
	isKnock_ = flag;
	actionTimer_.SetLimitTime(100);
	actionTimer_.Reset();
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
		&EnemyShadow::Following,
		&EnemyShadow::Wander,
		&EnemyShadow::HideMove,
		&EnemyShadow::Attack,
		&EnemyShadow::JumpAttack,
		&EnemyShadow::KnockBack
	};

	if (isKnock_)
	{
		state_ = State::KnockBack;
	}


	//実行
	(this->*Action[(int32_t)state_])();

	ImGui::Begin("Enemy");

	ImGui::Text(stateName_.c_str());

	ImGui::End();
}

void EnemyShadow::DrawSprite()
{
	
}

void EnemyShadow::Idle()
{
	stateName_ = "Idle";
	actionTimer_.SetLimitTime(120);

	actionTimer_.AddTime(1);

	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Following;
		actionTimer_.Reset();
	}
}

void EnemyShadow::Following()
{
	stateName_ = "Following";
	actionTimer_.SetLimitTime(180);
	actionTimer_.AddTime(1);

	Vector3& pos = splayer_->GetWorldTransform()->position_;
	Vector3 EtoPVec = pos - obj_->WT_.position_;
	EtoPVec.y = 0;
	float length = EtoPVec.length();
	
	addVec_ = EtoPVec.normalize() * moveSpeed;

	if (length < followLength || actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		actionTimer_.Reset();
	}
}

void EnemyShadow::Wander()
{
	stateName_ = "Wander";
	addVec_.x += 0.05f;
}

void EnemyShadow::HideMove()
{
	stateName_ = "HideMove";
	addVec_.x -= 0.05f;
}

void EnemyShadow::Attack()
{
	stateName_ = "Attack";
	addVec_.z += 0.01f;
}

void EnemyShadow::JumpAttack()
{
	stateName_ = "JumpAttack";
	addVec_.z -= 0.001f;
}

void EnemyShadow::KnockBack()
{
	stateName_ = "KnockBack";
	//一定時間経てばノック状態からアイドル状態に戻る
	actionTimer_.AddTime(1);
	if (actionTimer_.GetIsEnd()) {
		state_ = State::Idle;
		isKnock_ = false;
		actionTimer_.Reset();
	}
}

void EnemyShadow::PriorityUpdate()
{
}
