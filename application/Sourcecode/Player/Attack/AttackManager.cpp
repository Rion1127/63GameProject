#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
#include <imgui.h>
#include "Player.h"

#include "AttackSlide.h"
#include "AttackFinishBreak.h"
#include "AttackAirAerialFinish.h"
#include "AttackAirSweep.h"

Player* AttackManager::player_ = nullptr;

AttackManager::AttackManager()
{
	comboNum = 0;
	//attacks_.emplace_back(std::move(std::make_unique<Attack1>()));
	isAttacking = false;
}

void AttackManager::Attack()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_B))
	{
		//�G�Ǝ����̋������v�Z
		CalculatePtoELength();
		//MAX_COMBO���comboNum����������΍U���ł���
		if (comboNum < MAX_COMBO)
		{
			isNextAttack_ = true;
		}
		else {
			isNextAttack_ = false;
		}
	}
}

void AttackManager::Update()
{
	//�ŏ��̍U��
	if (isNextAttack_) {
		//���b�N�I�����Ă���G�Ƃ̍����̋���
		float diffPosY = 0;
		if (lockOnEnemy_ != nullptr) {
			diffPosY = player_->GetWorldTransform()->position_.y - lockOnEnemy_->GetWorldTransform()->position_.y;
		}
		diffPosY = fabs(diffPosY);
		//�U�����Ă��Ȃ��Ȃ�U����������
		if (nowAttack_ == nullptr)
		{
			isNextAttack_ = false;
			if (player_->GetNowState()->GetId() == PlayerState::Idle ||
				player_->GetNowState()->GetId() == PlayerState::Move)
			{
				if (PtoELength_ >= 4.f)nowAttack_ = std::make_unique<AttackSlide>(player_);
				else if (diffPosY > 1.f)nowAttack_ = std::make_unique<AttackAirSweep>(player_);
				else nowAttack_ = std::make_unique<Attack1>(player_);
			}
			else if (player_->GetNowState()->GetId() == PlayerState::Jump)
			{
				nowAttack_ = std::make_unique<AttackAir1>(player_);
			}
			if (nowAttack_ != nullptr) {
				nowAttack_->SetLockOnActor(lockOnEnemy_);
				nowAttack_->Init();
				float picth = RRandom::RandF(0.7f, 1.5f);
				SoundManager::Play("SwingSE", false, 0.3f, picth);
			}
			comboNum++;
		}
	}

	
	if (nowAttack_ != nullptr)
	{
		//�U�����t���O
		isAttacking = true;
		//�U���X�V
		nowAttack_->SetLockOnActor(lockOnEnemy_);
		nowAttack_->Update();
		//maxTime�𒴂�����nextAttack_��nowAttack_�ɑ������
		if (nowAttack_->GetTimer().GetIsEnd())
		{
			//���b�N�I�����Ă���G�Ƃ̍����̋���
			float diffPosY = 0;
			if (lockOnEnemy_ != nullptr) {
				diffPosY = player_->GetWorldTransform()->position_.y - lockOnEnemy_->GetWorldTransform()->position_.y;
			}
			else {
				PtoELength_ = 0;
			}
			diffPosY = fabs(diffPosY);
			//2�R���{�ȍ~�̏���
			if (isNextAttack_) {
				if (nextAttack_ == nullptr)
				{
					isNextAttack_ = false;
					if (player_->GetNowState()->GetId() == PlayerState::Attack)
					{
						//���łɍU�����Ă���ꍇ�͎��̍U��������
						if (comboNum == 1) {
							if (PtoELength_ >= 4.f)nextAttack_ = std::make_unique<AttackSlide>(player_);
							else if (diffPosY > 1.f)nowAttack_ = std::make_unique<AttackAirSweep>(player_);
							else nextAttack_ = std::make_unique<Attack2>(player_);
						}
						if (comboNum == 2)nextAttack_ = std::make_unique<AttackFinishBreak>(player_);
					}
					else if (player_->GetNowState()->GetId() == PlayerState::AirAttack)
					{
						if (comboNum == 1)nextAttack_ = std::make_unique<AttackAir2>(player_);
						if (comboNum == 2)nextAttack_ = std::make_unique<AttackAirAerialFinish>(player_);
					}
				}
			}

			nowAttack_.swap(nextAttack_);
			//�U��������
			if (nowAttack_ != nullptr) {
				nowAttack_->SetLockOnActor(lockOnEnemy_);
				nowAttack_->Init();
				float picth = RRandom::RandF(0.7f, 1.5f);
				SoundManager::Play("SwingSE", false, 0.3f, picth);
			}
			//nextAttack_���������
			nextAttack_.reset();
			nextAttack_ = nullptr;
			comboNum++;
		}
	}
	else
	{
		isAttacking = false;
		comboNum = 0;
	}
}

void AttackManager::DrawDebug()
{
	static int32_t isColDisplay = 1;
	if (isColDisplay)
	{
		if (nowAttack_ != nullptr)
		{
			nowAttack_->DrawCol();
		}
	}

	ImGui::Begin("Attack");
	int num = (int)comboNum;
	ImGui::SliderInt("combo", &num, 0, 10, "%d");
	int time = 0;
	if (nowAttack_ != nullptr) time = nowAttack_->GetTimer().GetTimer();
	ImGui::SliderInt("time", &time, 0, 120, "%d");
	if (ImGui::Button("colDisplay"))
	{
		isColDisplay++;
		isColDisplay = isColDisplay & 1;
	}
	ImGui::End();
}

void AttackManager::CalculatePtoELength()
{
	if (lockOnEnemy_ != nullptr) {
		Vector3 PtoEVec =
			player_->GetWorldTransform()->position_ - lockOnEnemy_->GetWorldTransform()->position_;

		PtoELength_ = PtoEVec.length();
	}
}
