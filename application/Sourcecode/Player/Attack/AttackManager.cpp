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

#include <fstream>
#include <iostream>

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
	}
}

void AttackManager::Update()
{
	//最初の攻撃
	if (isNextAttack_) {
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
			//nowAttack_->DrawCol();
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
		isNextAttack_ = false;
		if (player_->GetNowState()->GetId() == PlayerState::Idle ||
			player_->GetNowState()->GetId() == PlayerState::Move)
		{
			//遠くの敵にスライドして攻撃
			if (PtoELength_ >= 4.f)nowAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["test"], player_, lockOnEnemy_);
			//ジャンプして攻撃
			else if (diffPosY > 1.f)nowAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["test"], player_, lockOnEnemy_);
			//通常攻撃
			else nowAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Ground1"], player_, lockOnEnemy_);
		}
		else if (player_->GetNowState()->GetId() == PlayerState::Jump)
		{
			nowAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Air1"], player_, lockOnEnemy_);
		}
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
	if (isNextAttack_) {
		if (nextAttack_ == nullptr)
		{
			isNextAttack_ = false;
			if (player_->GetNowState()->GetId() == PlayerState::Attack)
			{
				//すでに攻撃している場合は次の攻撃を入れる
				if (comboNum == 1) {
					//遠くの敵にスライドして攻撃
					if (PtoELength_ >= 4.f)nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Ground2"], player_, lockOnEnemy_);
					//ジャンプして攻撃
					else if (diffPosY > 1.f)nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Ground2"], player_, lockOnEnemy_);
					//通常攻撃
					else nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Ground2"], player_, lockOnEnemy_);
				}
				//フィニッシュ攻撃（エクスプロージョン）
				if (comboNum == 2)nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Ground3"], player_, lockOnEnemy_);
			}
			else if (player_->GetNowState()->GetId() == PlayerState::AirAttack)
			{
				//空中2コンボ目
				if (comboNum == 1)nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Air2"], player_, lockOnEnemy_);
				//空中フィニッシュ
				if (comboNum == 2)nextAttack_ = std::make_unique<BaseAttack>(datapool_.GetAttacks()["Air3"], player_, lockOnEnemy_);
			}
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

AttackDataPool::AttackDataPool()
{
	LoadAllAttackFile();

	/*LoadAttackFile("test");
	LoadAttackFile("Air1");
	LoadAttackFile("Air2");
	LoadAttackFile("Air3");
	LoadAttackFile("Ground1");
	LoadAttackFile("Ground2");
	LoadAttackFile("Ground3");*/

	std::string saveDir = "application/Resources/AttackInfo/";
	std::vector<std::string> attackFileNames = FindFileNames(saveDir, ".csv",false);
	//存在しているファイルを全て読み込む
	for (auto& attackFileName : attackFileNames) {
		LoadAttackFile(attackFileName);
	}
}

void AttackDataPool::LoadAllAttackFile()
{
}

void AttackDataPool::LoadAttackFile(std::string fileName)
{
	std::string saveDir = "application/Resources/AttackInfo/";
	saveDir.append(fileName.c_str());
	saveDir += ".csv";

	std::ifstream file(saveDir);  // 読み込むファイルのパスを指定
	std::string line;

	BaseAttack::AttackInput newinput;
	newinput.attackinfo.emplace_back();
	auto& info = newinput.attackinfo.back();

	while (std::getline(file, line))
	{  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//各パラメータ読み込み
		if (key == "attackFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info.attackFrame;
		}
		else if (key == "gapFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info.gapFrame;

			info.attackAllFrame = info.attackFrame + info.gapFrame;
		}
		else if (key == "damege")
		{
			line_stream.ignore(1, '=');
			line_stream >> info.damage;
		}
		else if (key == "gravityY")
		{
			line_stream.ignore(1, '=');
			line_stream >> info.gravity.y;
		}
		//タイマー制御方法読み込み
		if (key == "TimerType")
		{
			std::string timerTypeName;
			line_stream >> timerTypeName;

			Spline::TimerType timerType;
			if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
			else timerType = Spline::TimerType::Easing;
			info.timerType = timerType;
		}
		//イージングの種類読み込み
		if (key == "EasingType")
		{
			std::string easingType_;
			line_stream >> easingType_;

			for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
			{
				if (easingType_ == Spline::GetEaseTypeNames()[i])
				{
					info.easingType = (Spline::EasingType)i;
					break;
				}
			}
		}
		//イージング・インアウト読み込み
		if (key == "EasingTypeInOut")
		{
			std::string easingTypeInOut_;
			line_stream >> easingTypeInOut_;

			Spline::EasingTypeInOut typeInout;
			if (easingTypeInOut_ == "In")typeInout = Spline::EasingTypeInOut::In;
			else if (easingTypeInOut_ == "Out")typeInout = Spline::EasingTypeInOut::Out;
			else typeInout = Spline::EasingTypeInOut::InOut;

			info.inOutType = typeInout;
		}
		//スプライン曲線読み込み
		if (key == "SplinePos")
		{
			info.splinePos.emplace_back();
			auto& splinePos = info.splinePos.back();

			line_stream >> splinePos.x;
			line_stream >> splinePos.y;
			line_stream >> splinePos.z;
		}
	}
	
	attacks_.insert(std::make_pair(fileName, newinput));
}
