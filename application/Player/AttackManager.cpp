#include "AttackManager.h"
#include <imgui.h>

AttackManager::AttackManager()
{
	controller_ = Controller::GetInstance();
	comboNum = 0;
	attacks_.insert(std::pair("First",std::move(std::make_unique<FirstAttack>())));
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
				nowAttack_ = std::move(std::make_unique<FirstAttack>());

				timer_ = 0;
				comboNum++;
			}
			else
			{
				//‚·‚Å‚ÉUŒ‚‚µ‚Ä‚¢‚éê‡‚ÍŸ‚ÌUŒ‚‚ğ“ü‚ê‚é
				nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 1)nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 2)nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 3)nextAttack_ = std::move(std::make_unique<SecondAttack>());
			}
		}
	}

	if (nowAttack_ != nullptr)
	{
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

			nextAttack_.reset();
			nextAttack_ = nullptr;
			comboNum++;
		}
	}
	else if (nowAttack_ == nullptr)
	{
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
	ImGui::SliderInt("combo %d", &num, 0, 10, "%d");
	int time = (int)timer_;
	ImGui::SliderInt("time %d", &time, 0, 120, "%d");

	ImGui::End();
}
