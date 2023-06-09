#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
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
	IAttack::SetLockOnActor(lockOnEnemy_);
	if (controller_->GetTriggerButtons(PAD::INPUT_B))
	{
		if (comboNum < MAX_COMBO)
		{
			//攻撃していないなら攻撃を代入する
			if (nowAttack_ == nullptr)
			{

				if (*IAttack::GetPlayerInfo()->state == PlayerState::Idle)
				{
					nowAttack_ = std::move(std::make_unique<Attack1>());
				}
				else if (*IAttack::GetPlayerInfo()->state == PlayerState::Jump)
				{
					nowAttack_ = std::move(std::make_unique<AttackAir1>());
				}
				if (nowAttack_ != nullptr) {
					nowAttack_->Init();
					float picth = RRandom::RandF(0.7f, 1.5f);
					SoundManager::Play("SwingSE", false, 0.3f,picth);
				}
				timer_ = 0;
				comboNum++;
			}
			else
			{
				if (nextAttack_ == nullptr)
				{
					if (*IAttack::GetPlayerInfo()->state == PlayerState::Attack)
					{
						//すでに攻撃している場合は次の攻撃を入れる
						if (comboNum == 1)nextAttack_ = std::move(std::make_unique<Attack2>());
						if (comboNum == 2)nextAttack_ = std::move(std::make_unique<Attack3>());
					}
					else if (*IAttack::GetPlayerInfo()->state == PlayerState::AirAttack)
					{
						if (comboNum == 1)nextAttack_ = std::move(std::make_unique<AttackAir2>());
						if (comboNum == 2)nextAttack_ = std::move(std::make_unique<AttackAir3>());
					}
				}
			}
		}
	}

	if (nowAttack_ != nullptr)
	{
		//攻撃中フラグ
		isAttacking = true;
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
			//攻撃初期化
			if (nowAttack_ != nullptr) {
				nowAttack_->Init();
				float picth = RRandom::RandF(0.7f, 1.5f);
				SoundManager::Play("SwingSE", false, 0.3f, picth);
			}
			//nextAttack_を解放する
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
