#include "AttackManager.h"
#include <imgui.h>

AttackManager::AttackManager()
{
	controller_ = Controller::GetInstance();
	comboNum = 0;
	attacks_.emplace_back(std::move(std::make_unique<Attack1>()));
	isAttacking = false;
}

void AttackManager::Update()
{
	if (controller_->GetTriggerButtons(PAD::INPUT_B))
	{
		if (comboNum < MAX_COMBO)
		{
			//UŒ‚‚µ‚Ä‚¢‚È‚¢‚È‚çUŒ‚‚ğ‘ã“ü‚·‚é
			if (nowAttack_ == nullptr)
			{
				
				if (*IAttack::GetPlayerInfo()->state == PlayerState::Idle) {
					nowAttack_ = std::move(std::make_unique<Attack1>());
				}
				else if (*IAttack::GetPlayerInfo()->state == PlayerState::Jump) {
					nowAttack_ = std::move(std::make_unique<AttackAir1>());
				}

				timer_ = 0;
				comboNum++;
			}
			else
			{
				if (*IAttack::GetPlayerInfo()->state == PlayerState::Attack) {
					//‚·‚Å‚ÉUŒ‚‚µ‚Ä‚¢‚éê‡‚ÍŸ‚ÌUŒ‚‚ğ“ü‚ê‚é
					if (comboNum == 1)nextAttack_ = std::move(std::make_unique<Attack2>());
					if (comboNum == 2)nextAttack_ = std::move(std::make_unique<Attack3>());
				}
				else if (*IAttack::GetPlayerInfo()->state == PlayerState::AirAttack) {
					if (comboNum == 1)nextAttack_ = std::move(std::make_unique<AttackAir2>());
					if (comboNum == 2)nextAttack_ = std::move(std::make_unique<AttackAir3>());
				}
			}
		}
	}

	if (nowAttack_ != nullptr)
	{
		//UŒ‚’†ƒtƒ‰ƒO
		isAttacking = true;
		//UŒ‚XV
		nowAttack_->Update();
		nowAttack_->SetNowTime(timer_);
		//UŒ‚’†ƒJƒEƒ“ƒg‚ği‚ß‚é
		timer_++;
		//maxTime‚ğ’´‚¦‚½‚çnextAttack_‚ğnowAttack_‚É‘ã“ü‚·‚é
		if (nowAttack_->GetInfo().maxTime <= timer_)
		{
			timer_ = 0;
			nowAttack_.swap(nextAttack_);
			//nextAttack_‚ğ‰ğ•ú‚·‚é
			nextAttack_.reset();
			nextAttack_ = nullptr;
			comboNum++;
		}
	}
	else if (nowAttack_ == nullptr)
	{
		isAttacking = false;
		comboNum = 0;
	}

	
}

void AttackManager::DrawDebug()
{
	if (nowAttack_ != nullptr)
	{
		nowAttack_->DrawCol();
	}

	ImGui::Begin("Attack");
	int num = (int)comboNum;
	ImGui::SliderInt("combo", &num, 0, 10, "%d");
	int time = (int)timer_;
	ImGui::SliderInt("time", &time, 0, 120, "%d");

	ImGui::End();
}
