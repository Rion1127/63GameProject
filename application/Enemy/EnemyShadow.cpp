#include "EnemyShadow.h"
#include "mInput.h"
#include "Player.h"
#include <imgui.h>
#include "RRandom.h"

#include "AttackShadow.h"

EnemyShadow::EnemyShadow(Vector3 pos) :
	IEnemy(EnemyType::Ground, true, 100)
{
	obj_ = std::move(std::make_unique<Object3d>());
	obj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	//obj_->SetAmbient("player", { 0,0,1.0f });
	knockResist_ = { 1,1,1 };

	obj_->GetTransform()->SetPosition(pos);
	damageCoolTime_.SetLimitTime(30);
	ColPosUpdate();

	state_ = State::Idle;
	actionTimer_.SetLimitTime(200);

	followLength = 5.f;
	moveSpeed = 0.1f;
	randRange_ = 30;
	isWanderInit_ = false;
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

void EnemyShadow::Draw()
{
	obj_->Draw();
	if (attack_ != nullptr) {
		attack_->DrawCol();
	}
}

void EnemyShadow::MoveUpdate()
{
	if (health_ < 100)
	{
		//health_ += 5;
	}
	//�X�e�[�^�X���Ƃ̓�����ǉ�
	void (EnemyShadow:: * Action[]) () =
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
	//(this->*Action[(int32_t)State::Attack])();

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

	/*if (actionTimer_.GetIsEnd())
	{
		state_ = State::Attack;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(70);
		attack_.reset();
		attack_ = std::move(std::make_unique<AttackShadow>(this, splayer_));
		attack_->Init();
	}*/

	if (actionTimer_.GetIsEnd())
	{
		state_ = State::Wander;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(300);
		isWanderInit_ = true;
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
	//�n�ʂɐ����Ĉړ�����
	stateName_ = "Wander";
	WanderInit();
	col_.isActive = false;

	//�ړ�����
	if (spline_.GetisEnd() == false)
	{
		if (obj_->WT_.scale_.y > 0.1f) {
			obj_->WT_.scale_.y -= 0.1f;
		}
		else {
			//�X�v���C���Ȑ��X�V
			spline_.Update();
			obj_->WT_.SetPosition(spline_.GetNowPoint());
		}
		Vector2 dir = {
			spline_.GetHeadingVec().x,
			spline_.GetHeadingVec().z
		};
		//�i�s�����ɉ�]
		float rotY = Vec2Angle(dir);
		obj_->WT_.rotation_.y = Radian(rotY);
	}
	//�ړ����I�������ʂ̃p�^�[����
	else {
		if (obj_->WT_.scale_.y < 1.0f) {
			obj_->WT_.scale_.y += 0.1f;
		}
		else {
			state_ = State::Idle;
			actionTimer_.Reset();
			actionTimer_.SetLimitTime(100);
			col_.isActive = true;
		}
	}
}

void EnemyShadow::HideMove()
{
	stateName_ = "HideMove";

}

void EnemyShadow::Attack()
{
	stateName_ = "Attack";
	attack_->Update();
	attack_->SetNowTime(actionTimer_.GetTimer());
	actionTimer_.SetLimitTime(attack_->GetInfo().maxTime);

	actionTimer_.AddTime(1);
	if (actionTimer_.GetIsEnd()) {
		state_ = State::Idle;
		actionTimer_.Reset();
		actionTimer_.SetLimitTime(150);
		attack_.reset();
	}
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
	attack_.reset();
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

void EnemyShadow::WanderInit()
{
	if (isWanderInit_) {
		//�폜����
		spline_.DleteAllPoint();
		spline_.Reset();
		//�����n�_��}��
		spline_.AddPosition(obj_->WT_.position_, PosState::Start);

		//�ړ�����|�C���g���v�Z����
		for (int32_t i = 0; i < 2; i++) {
			size_t index = spline_.GetsplinePos().size() - 1;
			Vector3 splinePos = spline_.GetsplinePos().at(index);
			//�X�v���C���̃|�C���g����targetPos�ւ̃x�N�g�����v�Z
			Vector3 targetPos = splayer_->GetWorldTransform()->position_;
			targetPos += {
				RRandom::RandF(-2.f, 2.f),
				0,
				RRandom::RandF(-2.f, 2.f)
			};
			Vector3 EtoPVec = targetPos - splinePos;
			EtoPVec = EtoPVec.normalize();

			EtoPVec = EtoPVec * Vector3(
				RRandom::RandF(3.f, 7.f),
				0,
				RRandom::RandF(3.f, 7.f));
			//����

			splinePos += EtoPVec;

			spline_.AddPosition(splinePos, PosState::Middle);
		}

		size_t index = spline_.GetsplinePos().size() - 1;
		Vector3 splinePos = spline_.GetsplinePos().at(index);
		//�ŏI�n�_�̓v���C���[�̋߂��֍s��
		Vector3 endPos = {
			splinePos.x + RRandom::RandF(-3.f, 3.f),
			0,
			splinePos.z + RRandom::RandF(-3.f, 3.f),
		};
		//�ŏI�n�_����
		spline_.AddPosition(endPos, PosState::End);
		spline_.SetIsStart(true);
		spline_.SetLimitTime(50);

		isWanderInit_ = false;
	}
}
