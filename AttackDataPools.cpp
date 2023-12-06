#include "AttackDataPools.h"
#include <fstream>
#include <iostream>

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
			info->attackType = timerType;
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

		if (key == "//--SeparateCol--//")
		{
			info->colType_ = ColType::Separate;
		}

		//各パラメータ読み込み
		if (key == "colattackFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->colInfo.attackFrame;
		}
		else if (key == "colactiveFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->colInfo.activeFrame;
		}
		else if (key == "coldamage")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->colInfo.damage;
		}
		else if (key == "colfirstRadius")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->colInfo.firstRadius;
		}
		else if (key == "coladdRadiusValue")
		{
			line_stream.ignore(1, '=');
			line_stream >> info->colInfo.addRadiusValue;
		}
		if (key == "colTimerType")
		{
			std::string timerTypeName;
			line_stream >> timerTypeName;

			Spline::TimerType timerType;
			if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
			else timerType = Spline::TimerType::Easing;
			info->colInfo.timerType = timerType;
		}
		//イージングの種類読み込み
		if (key == "colEasingType")
		{
			std::string easingType;
			line_stream >> easingType;

			for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
			{
				if (easingType == Spline::GetEaseTypeNames()[i])
				{
					info->colInfo.easingType = ((Spline::EasingType)i);
					break;
				}
			}
		}
		//イージング・インアウト読み込み
		if (key == "colEasingTypeInOut")
		{
			std::string easingTypeInOut;
			Spline::EasingTypeInOut typeInout;
			line_stream >> easingTypeInOut;

			if (easingTypeInOut == "In")typeInout = Spline::EasingTypeInOut::In;
			else if (easingTypeInOut == "Out")typeInout = Spline::EasingTypeInOut::Out;
			else typeInout = Spline::EasingTypeInOut::InOut;

			info->colInfo.inOutType = typeInout;
		}
		if (key == "//--ColSplinePos--//")
		{
			info->colInfo.splinePos.clear();
		}
		if (key == "ColSplinePos")
		{
			Vector3 pos;

			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			info->colInfo.splinePos.emplace_back(pos);
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