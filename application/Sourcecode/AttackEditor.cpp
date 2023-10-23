#include "AttackEditor.h"
#include <fstream>
#include <iostream>

/**
 * @file AttackEditor.cpp
 * @brief 攻撃を制作するエディタークラス
 */

AttackEditor::AttackEditor()
{
	playerObj_ = std::make_unique<Object3d>();
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	playerObj_->SetPos(Vector3(0, 1, 0));

	swordObj_ = std::make_unique<Sword>();
	
	isPlay_ = false;
	isAllPlay_ = false;
	isPointErase_ = false;

	//現在存在しているファイルを全て検索する
	FindAttackFile();

	currentSwingNum_ = 0;
	attackInfo_.emplace_back();
	attackInfo_[currentSwingNum_].deceleration = 0.1f;

	splinePointPos_.emplace_back();

	swordObj_->SetParent(playerObj_.get());

	spline_.SetParent(playerObj_->GetTransform());
}

void AttackEditor::Update()
{
	if (isPointErase_)
	{
		isPointErase_ = false;
		splinePointPos_[currentSwingNum_].erase(splinePointPos_[currentSwingNum_].begin() + (splinePointPos_[currentSwingNum_].size() - 1));
	}
	//選択している一振りだけ再生
	if (isPlay_)
	{
		timer_.AddTime(1);
		Vector3 pos = spline_.GetNowPoint() - playerObj_->GetPos();
		swordObj_->SetPos(pos);
		swordObj_->SetState(Sword::SwordState::Attack);

		if (timer_.GetIsEnd()) {
			isPlay_ = false;
			swordObj_->SetState(Sword::SwordState::Idle);
			spline_.SetIsStart(false);
		}
	}
	//選択している攻撃全てを再生
	if (isAllPlay_) {
		timer_.AddTime(1);

		swordObj_->SetPos(spline_.GetNowPoint());
		swordObj_->SetState(Sword::SwordState::Attack);

		if (timer_.GetIsEnd()) {
			if (currentSwingNum_ < attackInfo_.size() - 1) {
				currentSwingNum_++;
				AttackPlay();
			}
			else {
				isAllPlay_ = false;
				swordObj_->SetState(Sword::SwordState::Idle);
				spline_.SetIsStart(false);
			}
		}
	}
	if (isAllPlay_ == false && isPlay_ == false) {
		if (isValueChange_) {
			SetSplinePos();
			isValueChange_ = false;
		}
	}
	spline_.Update();

	Vector3 playerPos = playerObj_->GetPos();
	playerPos += moveVec_;
	MoveTo(Vector3(0, 0, 0), attackInfo_[currentSwingNum_].deceleration, moveVec_);
	playerObj_->SetPos(playerPos);

	swordObj_->EditorUpdate(spline_.GetNowPoint());

	playerObj_->Update();
}

void AttackEditor::Draw()
{
	playerObj_->Draw();

	swordObj_->Draw();

	spline_.DrawDebug();
}

void AttackEditor::DrawImGui()
{
	//プレイヤー座標の変更を行う
	ImGui::Begin("AttackEditor");

	static float playerpos[3] = { 0,1,0 };

	ImGui::DragFloat3("pos", playerpos, 0.1f, 100.f, 100.f);
	//playerObj_->SetPos(Vector3(playerpos[0], playerpos[1], playerpos[2]));

	if (ImGui::Button("PlayerPosReset"))
	{
		playerObj_->SetPos(Vector3(playerpos[0], playerpos[1], playerpos[2]));
	}

	ImGuiSave();
	ImGuiLoad();

	if (ImGui::Button("FileReload"))
	{
		FindAttackFile();
	}

	ImGui::End();

	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackSpline");
	ImGuiPlay();
	ImGui::SameLine();
	ImGuiAllPlay();
	if (ImGui::Button("AddSplinePoint"))
	{
		ImGuiADDSplinePos();
	}
	if (ImGui::Button("DeleteSplinePoint"))
	{
		if (splinePointPos_.size() > 0)
		{
			isPointErase_ = true;
		}
	}
	if (ImGui::Button("SplineTimerType"))
	{
		Spline::TimerType timerType;
		bool flag = (attackInfo_[currentSwingNum_].timerType == Spline::TimerType::Easing);

		timerType = flag ? Spline::TimerType::Normal : Spline::TimerType::Easing;

		attackInfo_[currentSwingNum_].timerType = timerType;
	}
	std::string timerType;
	if (attackInfo_[currentSwingNum_].timerType == Spline::TimerType::Easing) timerType = "Easing";
	else timerType = "Normal";
	ImGui::SameLine();
	ImGui::Text(timerType.c_str());

	ImGuiSetEasingType();
	ImGuiSetEasingTypeInOut();
	ImGuiDisplaySplitePoint();

	ImGui::End();

	ImGuiSwingCount();

	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackInfo");

	ImGui::DragFloat("attackFrame", &attackInfo_[currentSwingNum_].attackFrame, 1.0f, 0.f, 500.f);
	ImGui::DragFloat("gapFrame", &attackInfo_[currentSwingNum_].gapFrame, 1.0f, 0.f, 500.f);
	ImGui::DragInt("Damage", &attackInfo_[currentSwingNum_].damage, 1, 0, 500);
	ImGui::DragFloat("gravity", &attackInfo_[currentSwingNum_].gravity.y, 1.0f, 0.f, 500.f);

	float playerMoveVec[3] = {
		attackInfo_[currentSwingNum_].playerMoveVec.x,
		attackInfo_[currentSwingNum_].playerMoveVec.y,
		attackInfo_[currentSwingNum_].playerMoveVec.z,
	};
	ImGui::DragFloat3("playerMoveVec", playerMoveVec, 0.01f, 0.f, 500.f);
	attackInfo_[currentSwingNum_].playerMoveVec = { playerMoveVec[0],playerMoveVec[1], playerMoveVec[2] };
	ImGui::DragFloat("deceleration", &attackInfo_[currentSwingNum_].deceleration, 0.001f, 0.001f, 500.f);

	float knockVec[3] = {
		attackInfo_[currentSwingNum_].knockVec.x,
		attackInfo_[currentSwingNum_].knockVec.y,
		attackInfo_[currentSwingNum_].knockVec.z,
	};
	ImGui::DragFloat3("knockVec", knockVec, 0.01f, 0.f, 500.f);
	attackInfo_[currentSwingNum_].knockVec = { knockVec[0],knockVec[1], knockVec[2] };

	ImGui::End();
}

void AttackEditor::ImGuiDisplaySplitePoint()
{
	if (ImGui::CollapsingHeader("SplinePointPosision"))
	{
		int32_t splinePosIndex = 0;
		std::string splinePosName;
		Vector3 prePos;
		for (auto& spline : splinePointPos_[currentSwingNum_])
		{
			prePos = spline->splinePointPos_;
			std::ostringstream num;

			num << splinePosIndex;
			splinePosName = "Sprine" + num.str();

			float splinePos[3] = {
				spline->splinePointPos_.x,
				spline->splinePointPos_.y,
				spline->splinePointPos_.z,
			};
			ImGui::DragFloat3(splinePosName.c_str(), splinePos, 0.1f, 100.f, 100.f,"%.3f");
			spline->splinePointPos_ = { splinePos[0], splinePos[1],  splinePos[2] };
			if (prePos != spline->splinePointPos_)isValueChange_ = true;
			splinePosIndex++;
		}
	}
}

void AttackEditor::ImGuiSetEasingType()
{
	easingTypeNames.emplace_back("Back");
	easingTypeNames.emplace_back("Bounce");
	easingTypeNames.emplace_back("Circ");
	easingTypeNames.emplace_back("Quint");
	easingTypeNames.emplace_back("Cubic");
	easingTypeNames.emplace_back("Sine");
	//プルダウンメニューでイージングタイプを選択できるようにする
	if (ImGui::BeginCombo("EasingType", easingType_.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (easingType_ == Spline::GetEaseTypeNames()[i]);
			if (ImGui::Selectable(Spline::GetEaseTypeNames()[i].c_str(), is_selected))
			{
				easingType_ = Spline::GetEaseTypeNames()[i].c_str();
				attackInfo_[currentSwingNum_].easingType = ((Spline::EasingType)i);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void AttackEditor::ImGuiSetEasingTypeInOut()
{
	std::vector<std::string> easingTypenames;
	easingTypenames.emplace_back("In");
	easingTypenames.emplace_back("Out");
	easingTypenames.emplace_back("InOut");
	//プルダウンメニューでイージングタイプインアウトを選択できるようにする
	if (ImGui::BeginCombo("InOut", easingTypeInOut_.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingTypeInOut::EasingTypeInOutEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (easingTypeInOut_ == easingTypenames[i]);
			if (ImGui::Selectable(easingTypenames[i].c_str(), is_selected))
			{
				easingTypeInOut_ = easingTypenames[i].c_str();
				attackInfo_[currentSwingNum_].inOutType = ((Spline::EasingTypeInOut)i);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void AttackEditor::ImGuiADDSplinePos(const Vector3& pos)
{
	std::unique_ptr<SplinePos> newObj = std::make_unique<SplinePos>();
	newObj->splinePointPos_ = pos;
	splinePointPos_[currentSwingNum_].emplace_back(std::move(newObj));
	SetSplinePos();
}

void AttackEditor::ImGuiSave()
{
	static std::string saveName;		//書き出すファイルの名前
	static bool isProofSave = false;	//セーブの確認
	ImGui::InputText("SaveName", saveName.data(), 10);
	//セーブするか確認
	if (isProofSave)
	{
		ImGui::Text("Do you want to save?");
		if (ImGui::Button("No", ImVec2(50, 50)))
		{
			isProofSave = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Yes", ImVec2(50, 50)))
		{
			isProofSave = false;
			AttackSave(saveName);
		}

	}
	else
	{
		if (ImGui::Button("Save", ImVec2(50, 50)))
		{
			isProofSave = true;

		}
	}
}

void AttackEditor::ImGuiLoad()
{
	static std::string LoadName;		//読み込むファイルの名前
	static bool isProofLoad = false;	//ロードの確認

	//プルダウンメニューでイージングタイプを選択できるようにする
	if (ImGui::BeginCombo("LoadFileName", LoadName.c_str()))
	{
		for (uint32_t i = 0; i < allAttackFileNames.size(); ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (LoadName == allAttackFileNames[i]);
			if (ImGui::Selectable(allAttackFileNames[i].c_str(), is_selected))
			{
				LoadName = allAttackFileNames[i].c_str();
				spline_.SetEasingType_((Spline::EasingType)i);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	//ロードするか確認
	if (isProofLoad)
	{
		ImGui::Text("Do you want to Load?");
		if (ImGui::Button("No", ImVec2(50, 50)))
		{
			isProofLoad = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Yes", ImVec2(50, 50)))
		{
			isProofLoad = false;
			AttackLoad(LoadName);
		}
	}
	else
	{
		if (ImGui::Button("Load", ImVec2(50, 50)))
		{
			isProofLoad = true;
		}
	}
}

void AttackEditor::ImGuiPlay()
{
	if (ImGui::Button("Play", ImVec2(50, 50)))
	{
		if (splinePointPos_[currentSwingNum_].size() > 1)
		{
			isPlay_ = true;
			AttackPlay();
		}
	}
}

void AttackEditor::ImGuiAllPlay()
{
	if (ImGui::Button("AllPlay", ImVec2(60, 50)))
	{
		currentSwingNum_ = 0;
		if (splinePointPos_[currentSwingNum_].size() > 1)
		{
			isAllPlay_ = true;
			AttackPlay();
		}
	}
}

void AttackEditor::ImGuiSwingCount()
{
	ImGui::Begin("SwingCount");

	std::string swingCountStr;
	std::ostringstream num;

	num << attackInfo_.size();
	swingCountStr = "SwingCount = " + num.str();
	ImGui::Text(swingCountStr.c_str());

	if (ImGui::Button("Add",ImVec2(50,50))) {
		attackInfo_.emplace_back();
		splinePointPos_.emplace_back();
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete", ImVec2(50, 50))) {
		if (attackInfo_.size() > 1) {
			attackInfo_.erase(attackInfo_.begin() + (attackInfo_.size() - 1));
			splinePointPos_.erase(splinePointPos_.begin() + (splinePointPos_.size() - 1));
			currentSwingNum_--;
		}
	}
	int32_t preCurrentSwingNum = currentSwingNum_;
	ImGui::Text("currentSwingNum");
	ImGui::InputInt(" ", &currentSwingNum_);

	int32_t min = 0;
	int32_t max = (int32_t)(attackInfo_.size() - 1);
	currentSwingNum_ = Clamp(currentSwingNum_, min, max);
	//もし選択している数字が異なったら
	if (preCurrentSwingNum != currentSwingNum_) {
		auto& currentInfo = attackInfo_[currentSwingNum_];
		//プルダウンメニューの更新
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
		{
			if (currentInfo.easingType == Spline::EasingType(i))
			{
				easingType_ = Spline::GetEaseTypeNames()[i];
				break;
			}
		}

		std::string inoutString;
		if (currentInfo.inOutType == Spline::EasingTypeInOut::In)inoutString = "In";
		else if (currentInfo.inOutType == Spline::EasingTypeInOut::Out)inoutString = "Out";
		else inoutString = "InOut";
		
		easingTypeInOut_ = inoutString;
		SetSplinePos();
	}

	ImGui::End();
}

void AttackEditor::AttackSave(const std::string& string)
{
	std::string saveDir = "application/Resources/AttackInfo/";
	saveDir.append(string.c_str());
	saveDir += ".csv";
	std::ofstream writing_file;

	writing_file.open(saveDir, std::ios::out);

	uint16_t splineIndex = 0;

	for (auto& attackinfo : attackInfo_) {
		std::string writing_text = "//--AtatckInfo--//";
		writing_file << writing_text << std::endl;
		writing_text = "attackFrame";
		writing_file << writing_text << " = " << attackinfo.attackFrame << std::endl;
		writing_text = "gapFrame";
		writing_file << writing_text << " = " << attackinfo.gapFrame << std::endl;
		writing_text = "damege";
		writing_file << writing_text << " = " << attackinfo.damage << std::endl;
		writing_text = "gravityY";
		writing_file << writing_text << " = " << attackinfo.gravity.y << std::endl;
		Vector3& knockVec = attackinfo.knockVec;
		writing_text = "KnockVec";
		writing_file << writing_text << " " << knockVec.x << " " << knockVec.y << " " << knockVec.z << std::endl;
		Vector3& addVec = attackinfo.playerMoveVec;
		writing_text = "addVec";
		writing_file << writing_text << " " << addVec.x << " " << addVec.y << " " << addVec.z << std::endl;
		writing_text = "deceleration";
		writing_file << writing_text << " = " << attackinfo.deceleration << std::endl;
		writing_file << std::endl;

		writing_text = "//--SplinePos--//";
		writing_file << writing_text << std::endl;
		//タイマータイプを出力
		writing_text = "TimerType ";
		std::string timerType;
		//イージングの場合
		if (spline_.GetTimerType() == Spline::TimerType::Normal)
		{
			timerType = "Normal";
			writing_file << writing_text << timerType << std::endl;
		}
		else
		{
			std::string easingType;
			timerType = "Easing";
			writing_file << writing_text << timerType << std::endl;

			writing_text = "EasingType ";
			easingType = easingType_;
			writing_file << writing_text << easingType << std::endl;

			writing_text = "EasingTypeInOut ";
			easingType = easingTypeInOut_;
			writing_file << writing_text << easingTypeInOut_ << std::endl;
		}

		//スプライトの座標を出力する

		for (auto& spline : splinePointPos_[splineIndex])
		{
			Vector3& pos = spline->splinePointPos_;
			writing_text = "SplinePos";
			writing_file << writing_text << " " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		}
		splineIndex++;

		writing_file << std::endl;
	}
	writing_file.close();
	//現在存在しているファイルを全て検索する
	FindAttackFile();
}

void AttackEditor::AttackLoad(const std::string& string)
{
	splinePointPos_.clear();
	attackInfo_.clear();
	std::string saveDir = "application/Resources/AttackInfo/";
	saveDir.append(string.c_str());
	saveDir += ".csv";

	std::ifstream file(saveDir);  // 読み込むファイルのパスを指定
	std::string line;

	AttackInfo* attackinfo = nullptr;
	std::vector<std::unique_ptr<SplinePos>>* splinePos = nullptr;
	currentSwingNum_ = -1;

	while (std::getline(file, line))
	{  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		if (key == "//--AtatckInfo--//")
		{
			attackInfo_.emplace_back();
			attackinfo = &attackInfo_.back();
		}
		if (attackinfo == nullptr) continue;
		//各パラメータ読み込み
		if (key == "attackFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->attackFrame;
		}
		else if (key == "gapFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->gapFrame;
		}
		else if (key == "damege")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->damage;
		}
		else if (key == "gravityY")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->gravity.y;
		}
		else if (key == "KnockVec")
		{
			line_stream >> attackinfo->knockVec.x;
			line_stream >> attackinfo->knockVec.y;
			line_stream >> attackinfo->knockVec.z;
		}
		else if (key == "addVec")
		{
			line_stream >> attackinfo->playerMoveVec.x;
			line_stream >> attackinfo->playerMoveVec.y;
			line_stream >> attackinfo->playerMoveVec.z;
		}
		else if (key == "deceleration")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->deceleration;
		}

		if (key == "//--SplinePos--//")
		{
			splinePointPos_.emplace_back();
			splinePos = &splinePointPos_.back();
			currentSwingNum_++;
		}
		//タイマー制御方法読み込み
		if (key == "TimerType")
		{
			std::string timerTypeName;
			line_stream >> timerTypeName;

			Spline::TimerType timerType;
			if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
			else timerType = Spline::TimerType::Easing;
			attackinfo->timerType = timerType;
		}
		//イージングの種類読み込み
		if (key == "EasingType")
		{
			line_stream >> easingType_;

			for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
			{
				if (easingType_ == Spline::GetEaseTypeNames()[i])
				{
					attackinfo->easingType = ((Spline::EasingType)i);
					break;
				}
			}
		}
		//イージング・インアウト読み込み
		if (key == "EasingTypeInOut")
		{
			Spline::EasingTypeInOut typeInout;
			line_stream >> easingTypeInOut_;

			if (easingTypeInOut_ == "In")typeInout = Spline::EasingTypeInOut::In;
			else if (easingTypeInOut_ == "Out")typeInout = Spline::EasingTypeInOut::Out;
			else typeInout = Spline::EasingTypeInOut::InOut;

			attackinfo->inOutType = typeInout;
		}
		//スプライン曲線読み込み
		if (key == "SplinePos")
		{
			Vector3 pos;

			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			ImGuiADDSplinePos(pos);
		}
	}
	SetSplinePos();
	currentSwingNum_ = 0;
}

void AttackEditor::FindAttackFile()
{
	std::string dir = "application/Resources/AttackInfo/";
	allAttackFileNames = FindFileNames(dir, ".csv", false);
}

void AttackEditor::AttackPlay()
{
	timer_.Reset();
	float limitTime = 
		attackInfo_[currentSwingNum_].attackFrame + attackInfo_[currentSwingNum_].gapFrame;
	timer_.SetLimitTime(limitTime);
	spline_.SetIsStart(true);
	SetSplinePos();
	moveVec_ = attackInfo_[currentSwingNum_].playerMoveVec;
}

void AttackEditor::SetSplinePos()
{
	spline_.AllClear();
	spline_.SetMaxTime(attackInfo_[currentSwingNum_].attackFrame);
	spline_.SetTimerType_(attackInfo_[currentSwingNum_].timerType);
	spline_.SetEasingType_(attackInfo_[currentSwingNum_].easingType);
	spline_.SetEasingTypeInOut_(attackInfo_[currentSwingNum_].inOutType);
	for (int32_t i = 0; i < splinePointPos_[currentSwingNum_].size(); i++)
	{
		if (i == 0)
		{
			spline_.AddPosition(splinePointPos_[currentSwingNum_][i]->splinePointPos_, PosState::Start);
		}
		else if (i == splinePointPos_[currentSwingNum_].size() - 1)
		{
			spline_.AddPosition(splinePointPos_[currentSwingNum_][i]->splinePointPos_, PosState::End);
		}
		else
		{
			spline_.AddPosition(splinePointPos_[currentSwingNum_][i]->splinePointPos_, PosState::Middle);
		}
	}
}
