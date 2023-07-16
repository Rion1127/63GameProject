#include "EnemyShadow.h"
#include "mInput.h"
#include "Player.h"
#include <imgui.h>
#include "RRandom.h"

EnemyShadow::EnemyShadow(Vector3 pos) : IEnemy(EnemyType::Ground, true, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//obj_->SetAmbient("player", { 0,0,1.0f });
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damegeCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(240);

	followLength = 5.f;
	moveSpeed = 0.1f;
	randRange_ = 30;
}

void EnemyShadow::SetIsNock(bool flag)
{
	isKnock_ = flag;
	actionTimer_.SetLimitTime(100);
	actionTimer_.Reset();
}

void EnemyShadow::SetState(State state)
{
	state_ = state;
	actionTimer_.Reset();
	int32_t randRange = RRandom::Rand(-randRange_, randRange_);
	if (State::Idle == state_) {
		actionTimer_.SetLimitTime(100 + randRange);
	}
	else if (State::Following == state_) {
		actionTimer_.SetLimitTime(150 + randRange);
	}
}

void EnemyShadow::MoveUpdate()
{
	if (health_ < 100)
	{
		health_ += 5;
	}
	//�X�e�[�^�X���Ƃ̓�����ǉ�
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

	UpdateVector();

	//���s
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
	
	actionTimer_.AddTime(1);
	
	float length = EtoPVec_.length();
	addVec_ = EtoPVec_.normalize() * moveSpeed;
	//��苗���܂ŋ߂Â�����OR��莞�Ԓǂ���������
	if (length < followLength || actionTimer_.GetIsEnd())
	{
		state_ = State::Idle;
		actionTimer_.Reset();
	}
}

void EnemyShadow::Wander()
{
	stateName_ = "Wander";
	
}

void EnemyShadow::HideMove()
{
	stateName_ = "HideMove";
	
}

void EnemyShadow::Attack()
{
	stateName_ = "Attack";
	
}

void EnemyShadow::JumpAttack()
{
	stateName_ = "JumpAttack";
	
}

void EnemyShadow::KnockBack()
{
	stateName_ = "KnockBack";
	//��莞�Ԍo�Ă΃m�b�N��Ԃ���A�C�h����Ԃɖ߂�
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

void EnemyShadow::UpdateVector()
{
	//�v���C���[�ւ̃x�N�g��
	Vector3& pos = splayer_->GetWorldTransform()->position_;
	EtoPVec_ = pos - obj_->WT_.position_;
	EtoPVec_.y = 0;
}
