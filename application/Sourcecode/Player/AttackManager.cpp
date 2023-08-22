#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
#include <imgui.h>
#include "Player.h"

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
		//MAX_COMBO���comboNum����������΍U���ł���
		if (comboNum < MAX_COMBO)
		{
			//�U�����Ă��Ȃ��Ȃ�U����������
			if (nowAttack_ == nullptr)
			{
				if (player_->GetNowState()->GetId() == PlayerState::Idle||
					player_->GetNowState()->GetId() == PlayerState::Move)
				{
					nowAttack_ = std::move(std::make_unique<Attack1>(player_));
				}
				else if (player_->GetNowState()->GetId() == PlayerState::Jump)
				{
					nowAttack_ = std::move(std::make_unique<AttackAir1>(player_));
				}
				if (nowAttack_ != nullptr) {
					nowAttack_->SetLockOnActor(lockOnEnemy_);
					nowAttack_->Init();
					float picth = RRandom::RandF(0.7f, 1.5f);
					SoundManager::Play("SwingSE", false, 0.3f, picth);
				}
				timer_ = 0;
				comboNum++;
			}
			else
			{
				if (nextAttack_ == nullptr)
				{
					if (player_->GetNowState()->GetId() == PlayerState::Attack)
					{
						//���łɍU�����Ă���ꍇ�͎��̍U��������
						if (comboNum == 1)nextAttack_ = std::move(std::make_unique<Attack2>(player_));
						if (comboNum == 2)nextAttack_ = std::move(std::make_unique<Attack3>(player_));
					}
					else if (player_->GetNowState()->GetId() == PlayerState::AirAttack)
					{
						if (comboNum == 1)nextAttack_ = std::move(std::make_unique<AttackAir2>(player_));
						if (comboNum == 2)nextAttack_ = std::move(std::make_unique<AttackAir3>(player_));
					}
				}
			}
		}
	}
}

void AttackManager::Update()
{
	if (nowAttack_ != nullptr)
	{
		//�U�����t���O
		isAttacking = true;
		//�U���X�V
		nowAttack_->SetLockOnActor(lockOnEnemy_);
		nowAttack_->Update();
		nowAttack_->SetNowTime(timer_);
		//�U�����J�E���g��i�߂�
		timer_++;
		//maxTime�𒴂�����nextAttack_��nowAttack_�ɑ������
		if (nowAttack_->GetInfo().maxTime <= timer_)
		{
			timer_ = 0;
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
	int time = (int)timer_;
	ImGui::SliderInt("time", &time, 0, 120, "%d");
	if (ImGui::Button("colDisplay"))
	{
		isColDisplay++;
		isColDisplay = isColDisplay & 1;
	}
	ImGui::End();
}
