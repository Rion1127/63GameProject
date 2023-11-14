#include "AttackManager.h"
#include "mSound.h"
#include "RRandom.h"
#include <imgui.h>
#include "Player.h"
#include "SoundVolume.h"

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
	if (isNextAttack_) {
		if (nextAttack_ == nullptr)
		{
			std::string keyName;
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
			if (keyName == "")return;
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

AttackDataPool::AttackDataPool()
{
	attackInfoDir_ = "application/Resources/Attack/AttackInfo/";
	attackKeyDir_ = "application/Resources/Attack/";
	std::vector<std::string> attackFileNames = FindFileNames(attackInfoDir_, ".csv", false);
	//存在しているファイルを全て読み込む
	for (auto& attackFileName : attackFileNames) {
		LoadAttackFile(attackFileName);
	}

	LoadattackKeys("AttackKey");
}

void AttackDataPool::LoadAttackFile(std::string fileName)
{
	std::string loadDir = attackInfoDir_;
	loadDir.append(fileName.c_str());
	loadDir += ".csv";

	std::ifstream file(loadDir);  // 読み込むファイルのパスを指定
	std::string line;

	BaseAttack::AttackData newinput;

	AttackEditor::AttackInfo* info = nullptr;
	AttackEditor::QuaternionControl* quaternionControl = nullptr;

	while (std::getline(file, line))
	{  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//各パラメータ読み込み
		if (key == "//--AtatckInfo--//")
		{
			newinput.attackinfo.emplace_back();
			info = &newinput.attackinfo.back();
		}
		if (info == nullptr)continue;
		if (key == "attackFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->attackFrame;
		}
		else if (key == "gapFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->gapFrame;

			info->attackAllFrame = info->attackFrame + info->gapFrame;
		}
		else if (key == "damege")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->damage;
		}
		else if (key == "gravityY")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->gravity.y;
		}
		else if (key == "KnockVec")
		{
			line_stream >> info->knockVec.x;
			line_stream >> info->knockVec.y;
			line_stream >> info->knockVec.z;
		}
		else if (key == "KnockYVec")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->knockYVec;
		}
		else if (key == "addVec")
		{
			line_stream >> info->playerMoveVec.x;
			line_stream >> info->playerMoveVec.y;
			line_stream >> info->playerMoveVec.z;
		}
		else if (key == "deceleration")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->deceleration;
		}

		if (key == "attackType")
		{
			std::string attackType;
			line_stream >> attackType;

			AttackType timerType;
			if (attackType == "Normal")timerType = AttackType::Normal;
			else timerType = AttackType::Finish;
			newinput.type_ = timerType;
		}

		//タイマー制御方法読み込み
		if (key == "TimerType")
		{
			std::string timerTypeName;
			line_stream >> timerTypeName;

			Spline::TimerType timerType;
			if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
			else timerType = Spline::TimerType::Easing;
			info->timerType = timerType;
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
					info->easingType = (Spline::EasingType)i;
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

			info->inOutType = typeInout;
		}
		//スプライン曲線読み込み
		if (key == "SplinePos")
		{
			info->splinePos.emplace_back();
			auto& splinePos = info->splinePos.back();

			line_stream >> splinePos.x;
			line_stream >> splinePos.y;
			line_stream >> splinePos.z;
		}

		if (key == "//--Quaternion--//")
		{
			
		}
		//if (quaternionControl == nullptr) continue;
		//スプライン曲線読み込み
		if (key == "flame")
		{
			info->quaternion.emplace_back();
			quaternionControl = &info->quaternion.back();

			quaternionControl->frame;
			float frame;
			line_stream >> frame;

			quaternionControl->frame = frame;
		}
		if (key == "Quaternion")
		{
			Quaternion q{};
			line_stream >> q.x;
			line_stream >> q.y;
			line_stream >> q.z;
			line_stream >> q.w;

			quaternionControl->q = q;
		}
	}

	attacks_.insert(std::make_pair(fileName, newinput));
}

void AttackDataPool::LoadattackKeys(std::string fileName)
{
	std::string loadDir = attackKeyDir_;
	loadDir.append(fileName.c_str());
	loadDir += ".csv";

	std::ifstream file(loadDir);  // 読み込むファイルのパスを指定
	std::string line;

	while (std::getline(file, line))
	{  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		std::string keyName;
		line_stream >> keyName;

		attackKeys_.insert(std::make_pair(key, keyName));
	}
}
