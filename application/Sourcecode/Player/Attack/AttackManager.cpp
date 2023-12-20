#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
#include <imgui.h>
#include "Player.h"
#include "SoundVolume.h"

/**
 * @file AttackManager.cpp
 * @brief プレイヤーの基本攻撃を管理するクラス
 */

Player* AttackManager::player_ = nullptr;

AttackManager::AttackManager()
{
	comboNum = 0;

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
		if (Controller::GetTriggerButtons(PAD::INPUT_Y))
		{
			//敵と自分の距離を計算
			CalculatePtoELength();
			//MAX_COMBOよりcomboNumが小さければ攻撃できる
			isNextSpecialAttack_ = true;
			isNextAttack_ = false;
		}
	}
}

void AttackManager::Update()
{
	//最初の攻撃
	if (isNextAttack_ || isNextSpecialAttack_) {
		FirstAttackUpdate();
	}

	if (nowAttack_ != nullptr) {
		isAttacking = true;
		nowAttack_->Update();
		if (nowAttack_->GetIsAttaking() == false) {
			SwitchAttack();
		}
	}
	else
	{
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
			nowAttack_->Draw();
		}
	}

	if (isSplinePosDisplay)
	{
		if (nowAttack_ != nullptr)
		{
			//nowAttack_->DrawSplinePoint();
		}
	}

	ImGui::Begin("Attack");
	int num = (int)comboNum;
	ImGui::SliderInt("combo", &num, 0, 10, "%d");
	int time = 0;
	//if (nowAttack_ != nullptr) time = (int)nowAttack_->GetTimer().GetTimer();
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
		std::string keyName;
		isNextAttack_ = false;
		
		if (player_->GetNowState()->GetId() == PlayerState::Idle ||
			player_->GetNowState()->GetId() == PlayerState::Move)
		{
			//遠くの敵にスライドして攻撃
			if (PtoELength_ >= 4.5f)keyName = datapool_.GetKeyName()["distant"];
			//ジャンプして攻撃
			else if (diffPosY > 1.5f)keyName = datapool_.GetKeyName()["JumpAttack"];
			//通常攻撃
			else keyName = datapool_.GetKeyName()["Ground1"];
		}
		else if (player_->GetNowState()->GetId() == PlayerState::Jump)
		{
			//空中攻撃
			keyName = datapool_.GetKeyName()["Air1"];
		}
		if (isNextSpecialAttack_) {
			isNextSpecialAttack_ = false;
			keyName = datapool_.GetKeyName()["Special1"];
		}
		if (keyName == "")return;
		//条件に合ったキーで攻撃を生成
		nowAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()[keyName], player_, lockOnEnemy_);

		if (nowAttack_ != nullptr) {
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
	if (GetIsNextAttack()) {
		if (nextAttack_ == nullptr)
		{
			std::string keyName;
			if (isNextAttack_) {
				isNextAttack_ = false;
				if (player_->GetNowState()->GetId() == PlayerState::Attack)
				{
					//すでに攻撃している場合は次の攻撃を入れる
					if (comboNum == 1) {
						//遠くの敵にスライドして攻撃
						if (PtoELength_ >= 4.f)keyName = datapool_.GetKeyName()["distant"];
						//ジャンプして攻撃
						else if (diffPosY > 1.5f)keyName = datapool_.GetKeyName()["JumpAttack"];
						//通常攻撃
						else keyName = datapool_.GetKeyName()["Ground2"];
					}
					//フィニッシュ攻撃（エクスプロージョン）
					if (comboNum == 2)keyName = datapool_.GetKeyName()["Ground3"];
				}
				else if (player_->GetNowState()->GetId() == PlayerState::AirAttack)
				{
					//空中2コンボ目
					if (comboNum == 1)keyName = datapool_.GetKeyName()["Air2"];
					//空中フィニッシュ
					if (comboNum == 2)keyName = datapool_.GetKeyName()["Air3"];
				}
			}

			if (isNextSpecialAttack_) {
				keyName = datapool_.GetKeyName()["Special1"];
				isNextSpecialAttack_ = false;
			}
			//条件に合ったキーで攻撃を生成
			nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()[keyName], player_, lockOnEnemy_);
		}
	}

	nowAttack_.swap(nextAttack_);
	//攻撃初期化
	if (nowAttack_ != nullptr) {
		float picth = RRandom::RandF(0.7f, 1.5f);
		SoundManager::Play("SwingSE", false, SoundVolume::GetValumeSE(), picth);
	}
	else {
		isAttacking = false;
	}
	//nextAttack_を解放する
	nextAttack_.reset();
	nextAttack_ = nullptr;
	comboNum++;
}

bool AttackManager::GetIsNextAttack()
{
	return (isNextAttack_ || isNextSpecialAttack_);
}
