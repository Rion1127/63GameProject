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
			//攻撃していないなら攻撃を代入する
			if (nowAttack_ == nullptr)
			{
				nowAttack_ = std::move(std::make_unique<FirstAttack>());

				timer_ = 0;
				comboNum++;
			}
			else
			{
				//すでに攻撃している場合は次の攻撃を入れる
				nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 1)nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 2)nextAttack_ = std::move(std::make_unique<SecondAttack>());
				if (comboNum == 3)nextAttack_ = std::move(std::make_unique<SecondAttack>());
			}
		}
	}

	if (nowAttack_ != nullptr)
	{
		//攻撃更新
		nowAttack_->Update();
		nowAttack_->SetNowTime(timer_);
		//攻撃中カウントを進める
		timer_++;
		//maxTimeを超えたらnextAttack_をnowAttack_に代入する
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
