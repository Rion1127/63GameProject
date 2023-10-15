#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
#include <imgui.h>
#include "Player.h"
#include "SoundVolume.h"

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
	if (player_->GetIsCanInput()) {
		if (Controller::GetTriggerButtons(PAD::INPUT_B))
		{
			//敵と自分の距離を計算
			CalculatePtoELength();
			//MAX_COMBOよりcomboNumが小さければ攻撃できる
			if (comboNum < MAX_COMBO)
			{
				isNextAttack_ = true;
			}
			else {
				isNextAttack_ = false;
			}
		}
	}
}

void AttackManager::Update()
{
	//最初の攻撃
	if (isNextAttack_) {
		FirstAttackUpdate();
	}

	if (nowAttack_ != nullptr)
	{
		//攻撃中フラグ
		isAttacking = true;
		//攻撃更新
		nowAttack_->SetLockOnActor(lockOnEnemy_);
		nowAttack_->Update();
		//maxTimeを超えたらnextAttack_をnowAttack_に代入する
		if (nowAttack_->GetTimer().GetIsEnd())
		{
			SwitchAttack();
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
	static int32_t isSplinePosDisplay = 1;
	if (isColDisplay)
	{
		if (nowAttack_ != nullptr)
		{
			nowAttack_->DrawCol();
		}
	}

	if (isSplinePosDisplay)
	{
		if (nowAttack_ != nullptr)
		{
			nowAttack_->DrawSplinePoint();
		}
	}

	ImGui::Begin("Attack");
	int num = (int)comboNum;
	ImGui::SliderInt("combo", &num, 0, 10, "%d");
	int time = 0;
	if (nowAttack_ != nullptr) time = (int)nowAttack_->GetTimer().GetTimer();
	ImGui::SliderInt("time", &time, 0, 120, "%d");
	//当たり判定表示
	if (ImGui::Button("colDisplay"))
	{
		isColDisplay++;
		isColDisplay = isColDisplay & 1;
	}
	std::string isColFlagstring;
	if (isColDisplay)isColFlagstring = "Display";
	else isColFlagstring = "Hide";

	ImGui::SameLine();
	ImGui::Text(isColFlagstring.c_str());
	//スプラインポイント表示
	if (ImGui::Button("SplinePosDisplay"))
	{
		isSplinePosDisplay++;
		isSplinePosDisplay = isSplinePosDisplay & 1;
	}
	std::string isSplinePosflagstring;

	if (isSplinePosDisplay)isSplinePosflagstring = "Display";
	else isSplinePosflagstring = "Hide";

	ImGui::SameLine();
	ImGui::Text(isSplinePosflagstring.c_str());

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

void AttackManager::FirstAttackUpdate()
{
	//ロックオンしている敵との高さの距離
	float diffPosY = 0;
	if (lockOnEnemy_ != nullptr) {
		diffPosY = lockOnEnemy_->GetWorldTransform()->position_.y - player_->GetWorldTransform()->position_.y;
	}
	//攻撃していないなら攻撃を代入する
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
			SoundManager::Play("SwingSE", false, SoundVolume::GetValumeSE(), picth);
		}
		comboNum++;
	}
}

void AttackManager::SwitchAttack()
{
	//ロックオンしている敵との高さの距離
	float diffPosY = 0;
	if (lockOnEnemy_ != nullptr) {
		diffPosY = player_->GetWorldTransform()->position_.y - lockOnEnemy_->GetWorldTransform()->position_.y;
	}
	else {
		PtoELength_ = 0;
	}
	diffPosY = fabs(diffPosY);
	//2コンボ以降の処理
	if (isNextAttack_) {
		if (nextAttack_ == nullptr)
		{
			isNextAttack_ = false;
			if (player_->GetNowState()->GetId() == PlayerState::Attack)
			{
				//すでに攻撃している場合は次の攻撃を入れる
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
	//攻撃初期化
	if (nowAttack_ != nullptr) {
		nowAttack_->SetLockOnActor(lockOnEnemy_);
		nowAttack_->Init();
		float picth = RRandom::RandF(0.7f, 1.5f);
		SoundManager::Play("SwingSE", false, SoundVolume::GetValumeSE(), picth);
	}
	//nextAttack_を解放する
	nextAttack_.reset();
	nextAttack_ = nullptr;
	comboNum++;
}
