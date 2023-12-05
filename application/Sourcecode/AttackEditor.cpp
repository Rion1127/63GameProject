#include "AttackEditor.h"
#include <fstream>
#include <iostream>

/**
 * @file AttackEditor.cpp
 * @brief 攻撃を制作するエディタークラス
 */

AttackEditor::AttackEditor()
{
	attackInfoDir_ = "application/Resources/Attack/AttackInfo/";
	attackKeyDir_ = "application/Resources/Attack/AttackKey";

	playerObj_ = std::make_unique<Object3d>();
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true,false));
	playerObj_->SetPos(Vector3(0, 1, 0));
	playerObj_->WT_.SetRotType(RotType::Quaternion);
	playerObj_->WT_.quaternion_ = IdentityQuaternion();
	
	displayPlayerObj_ = std::make_unique<Object3d>();
	displayPlayerObj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	displayPlayerObj_->SetShadowOffsetPos(Vector3(0,-1,0));
	displayPlayerObj_->SetPos(Vector3(0, 1, 0));
	displayPlayerObj_->WT_.SetRotType(RotType::Quaternion);
	displayPlayerObj_->WT_.quaternion_ = IdentityQuaternion();

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
	quaternions_.emplace_back();
	quaternions_[currentSwingNum_].emplace_back();

	swordObj_->SetParent(displayPlayerObj_.get());

	spline_.SetParent(playerObj_->GetTransform());
	//キー読み込み
	AttackKeyLoad();
	cullentQuaternion_ = IdentityQuaternion();

	spline_.SetIsLineDisplay(true);
}

void AttackEditor::Update()
{
	if (isPointErase_)
	{
		isPointErase_ = false;
		splinePointPos_[currentSwingNum_].erase(splinePointPos_[currentSwingNum_].begin() + (splinePointPos_[currentSwingNum_].size() - 1));
		SetSplinePos();
	}

	colSpline_.Update();
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

		QuaternionUpdate();
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

		QuaternionUpdate();
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
	displayPlayerObj_->SetPos(playerObj_->GetPos());
	displayPlayerObj_->Update();
	postureDisplay.Update(cullentQuaternion_);
}

void AttackEditor::Draw()
{
	displayPlayerObj_->Draw();

	swordObj_->Draw();

	spline_.DrawDebug();

	colSpline_.Draw();
}

void AttackEditor::DrawRenderTexture()
{
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	postureDisplay.Draw();
}

void AttackEditor::DrawImGui()
{
	//プレイヤー座標の変更を行う

	ImGui::Begin("PlayerPos");
	static float playerpos[3] = { 0,1,0 };

	ImGui::DragFloat3("pos", playerpos, 0.1f, 100.f, 100.f);

	if (ImGui::Button("PlayerPosReset"))
	{
		playerObj_->SetPos(Vector3(playerpos[0], playerpos[1], playerpos[2]));
		displayPlayerObj_->WT_.SetQuaternion(IdentityQuaternion());
	}
	ImGui::End();

	ImGui::Begin("File_Load,Save");
	
	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("PlayerPosReset : posの値にプレイヤーの座標を移動させる");
		ImGui::TextUnformatted("Save           : SaveNameに入力した名前でファイルを保存する");
		ImGui::TextUnformatted("保存ファイル場所 : \"application/Resources/Attack/AttackInfo/\"");
		ImGui::TextUnformatted("Load           : 選択したファイルを読み込む");
		ImGui::TextUnformatted("FileReload     : 外部からファイルを導入した際のホットリロード");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGuiSave();
	ImGuiLoad();

	if (ImGui::Button("FileReload"))
	{
		FindAttackFile();
	}

	ImGui::End();

	ImGuiSplineEditor();

	ImGuiSwingCount();

	ImGuiAttackInfo();

	ImGuiSettingCombo();
	//姿勢制御
	ImGuiQuaternion();

	colSpline_.DrawImGui();
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
			prePos = *spline;
			std::ostringstream num;

			num << splinePosIndex;
			splinePosName = "Sprine" + num.str();

			float splinePos[3] = {
				spline->x,
				spline->y,
				spline->z,
			};
			ImGui::DragFloat3(splinePosName.c_str(), splinePos, 0.1f, 100.f, 100.f,"%.3f");
			*spline = { splinePos[0], splinePos[1],  splinePos[2] };
			if (prePos != *spline)isValueChange_ = true;
			splinePosIndex++;
		}
	}
}

void AttackEditor::ImGuiADDSplinePos(const Vector3& pos, uint32_t index)
{
	std::unique_ptr<Vector3> newObj = std::make_unique<Vector3>();
	*newObj = pos;
	splinePointPos_[index].emplace_back(std::move(newObj));
	SetSplinePos();
}

void AttackEditor::ImGuiSave()
{
	static std::string saveName;		//書き出すファイルの名前
	static bool isProofSave = false;	//セーブの確認
	static bool isProofQuickSave = false;	//セーブの確認
	ImGui::InputText("SaveName", saveName.data(), 15);
	//セーブするか確認
	if (isProofSave || isProofQuickSave)
	{
		ImGui::Text("Do you want to save?");
		if (ImGui::Button("No", ImVec2(50, 50)))
		{
			isProofSave = false;
			isProofQuickSave = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Yes", ImVec2(50, 50)))
		{
			if (isProofSave)
			{
				isProofSave = false;
				AttackSave(saveName);
			}
			else
			{
				isProofQuickSave = false;
				AttackSave(loadFileName_);
			}
		}

	}
	else
	{
		if (ImGui::Button("Save", ImVec2(50, 50)))
		{
			isProofSave = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("QuickSave", ImVec2(70, 50)))
		{
			isProofQuickSave = true;
		}
	}
}

void AttackEditor::ImGuiLoad()
{
	static bool isProofLoad = false;	//ロードの確認

	//プルダウンメニューでイージングタイプを選択できるようにする
	if (ImGui::BeginCombo("LoadFileName", loadFileName_.c_str()))
	{
		for (uint32_t i = 0; i < allAttackFileNames.size(); ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (loadFileName_ == allAttackFileNames[i]);
			if (ImGui::Selectable(allAttackFileNames[i].c_str(), is_selected))
			{
				loadFileName_ = allAttackFileNames[i].c_str();
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
			AttackLoad(loadFileName_);
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
			colSpline_.PlaySpline();
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
			colSpline_.PlaySpline();
		}
	}
}

void AttackEditor::ImGuiSplineEditor()
{
	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackSpline");

	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("Play                : 現在選択しているスプライン曲線を再生する");
		ImGui::TextUnformatted("AllPlay             : SwingCount分のスプライン曲線を再生する");
		ImGui::TextUnformatted("AddSplinePoint      : 制御点を追加する");
		ImGui::TextUnformatted("DeleteSplinePoint   : 制御点を削除する");
		ImGui::TextUnformatted("SplineTimerType     : スプラインの進み方を変更する(Normal or Easing)");
		ImGui::TextUnformatted("EasingType          : イージングの種類を変更する");
		ImGui::TextUnformatted("InOut               : イージングのInOutの種類を変更する");
		ImGui::TextUnformatted("SplinePointPosition : 制御点を移動させる");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGuiPlay();
	ImGui::SameLine();
	ImGuiAllPlay();
	if (ImGui::Button("AddSplinePoint"))
	{
		ImGuiADDSplinePos(Vector3(0, 0, 0), currentSwingNum_);
	}
	if (ImGui::Button("DeleteSplinePoint"))
	{
		if (splinePointPos_.size() > 0)
		{
			isPointErase_ = true;
		}
	}

	configCommon_.SetTimerType(attackInfo_[currentSwingNum_].timerType);
	configCommon_.SetEasingType(attackInfo_[currentSwingNum_].easingType);
	configCommon_.SetEasingTypeInOut(attackInfo_[currentSwingNum_].inOutType);

	ImGuiDisplaySplitePoint();

	ImGui::End();
}

void AttackEditor::ImGuiAttackInfo()
{
	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackInfo");

	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("attackFrame         : スプライン曲線の時間");
		ImGui::TextUnformatted("gapFrame            : 攻撃後の後隙");
		ImGui::TextUnformatted("Damage              : 敵に与えるダメージ");
		ImGui::TextUnformatted("gravity             : 攻撃時のY座標の動き");
		ImGui::TextUnformatted("playerMoveVec       : プレイヤーの移動方向");
		ImGui::TextUnformatted("deceleration        : playerMoveVecの減速速度");
		ImGui::TextUnformatted("knockVec            : 敵のノックバックする方向");
		ImGui::TextUnformatted("knockVecY           : 上方向のノックバック");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	//攻撃のステータス
	ImGui::DragFloat("attackFrame", &attackInfo_[currentSwingNum_].attackFrame, 1.0f, 0.f, 500.f);
	ImGui::DragFloat("gapFrame", &attackInfo_[currentSwingNum_].gapFrame, 1.0f, 0.f, 500.f);
	ImGui::DragInt("Damage", &attackInfo_[currentSwingNum_].damage, 1, 0, 500);
	ImGui::DragFloat("gravity", &attackInfo_[currentSwingNum_].gravity.y, 1.0f, 0.f, 500.f);
	float playerMoveVec[3] = {
		attackInfo_[currentSwingNum_].playerMoveVec.x,
		attackInfo_[currentSwingNum_].playerMoveVec.y,
		attackInfo_[currentSwingNum_].playerMoveVec.z,
	};
	ImGui::DragFloat3("playerMoveVec", playerMoveVec, 0.01f, -10.f, 500.f);
	attackInfo_[currentSwingNum_].playerMoveVec = { playerMoveVec[0],playerMoveVec[1], playerMoveVec[2] };
	ImGui::DragFloat("deceleration", &attackInfo_[currentSwingNum_].deceleration, 0.001f, 0.001f, 500.f);
	float knockVec[3] = {
		attackInfo_[currentSwingNum_].knockVec.x,
		attackInfo_[currentSwingNum_].knockVec.y,
		attackInfo_[currentSwingNum_].knockVec.z,
	};
	ImGui::DragFloat3("knockVec", knockVec, 0.01f, 0.f, 500.f);
	attackInfo_[currentSwingNum_].knockVec = { knockVec[0],knockVec[1], knockVec[2] };
	ImGui::DragFloat("knockYVec", &attackInfo_[currentSwingNum_].knockYVec, 0.01f, -10.f, 10.f);
	//攻撃の種類（フィニッシュ攻撃かノーマル攻撃か）
	if (ImGui::Button("AttackType"))
	{
		AttackType timerType;
		bool flag = (attackInfo_[currentSwingNum_].attackType == AttackType::Normal);

		timerType = flag ? AttackType::Finish : AttackType::Normal;

		attackInfo_[currentSwingNum_].attackType = timerType;
	}
	std::string attackType;
	if (attackInfo_[currentSwingNum_].attackType == AttackType::Finish) attackType = "Finish";
	else attackType = "Normal";
	ImGui::SameLine();
	ImGui::Text(attackType.c_str());

	ImGui::End();
}

void AttackEditor::ImGuiSwingCount()
{
	ImGui::Begin("SwingCount");

	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("SwingCount       : 一回の攻撃で剣を振る回数");
		ImGui::TextUnformatted("Add              : 剣を振る回数を増やす");
		ImGui::TextUnformatted("Delete           : 剣を振る回数を減らす");
		ImGui::TextUnformatted("currentSwingNum   : 編集するスプライン曲線を選択する");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	std::string swingCountStr;
	std::ostringstream num;

	num << attackInfo_.size();
	swingCountStr = "SwingCount = " + num.str();
	ImGui::Text(swingCountStr.c_str());

	if (ImGui::Button("Add",ImVec2(50,50))) {
		attackInfo_.emplace_back();
		splinePointPos_.emplace_back();
		quaternions_.emplace_back();
	}
	ImGui::SameLine();
	//要素の削除
	if (ImGui::Button("Delete", ImVec2(50, 50))) {
		if (attackInfo_.size() > 1) {
			attackInfo_.erase(attackInfo_.begin() + (attackInfo_.size() - 1));
			splinePointPos_.erase(splinePointPos_.begin() + (splinePointPos_.size() - 1));
			quaternions_.erase(quaternions_.begin() + (quaternions_.size() - 1));
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
				configCommon_.SetEasingTypeName(Spline::GetEaseTypeNames()[i]);
				break;
			}
		}

		std::string inoutString;
		if (currentInfo.inOutType == Spline::EasingTypeInOut::In)inoutString = "In";
		else if (currentInfo.inOutType == Spline::EasingTypeInOut::Out)inoutString = "Out";
		else inoutString = "InOut";
		
		configCommon_.SetEasingTypeInOutName(inoutString);
		SetSplinePos();
	}

	ImGui::End();
}

void AttackEditor::ImGuiSettingCombo()
{
	ImGui::Begin("SettingCombo");

	ImGui::Text("HELP ");
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("KeyName    : どのコンボにセットするか選択する");
		ImGui::TextUnformatted("AttackName : セットする攻撃を選択する");
		ImGui::TextUnformatted("Set        : 選択した攻撃をセットする");
		ImGui::TextUnformatted("Save       : ファイルに出力する");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	static std::string setKeyName;		//読み込むファイルの名前
	//プルダウンメニューでキーを選択
	if (ImGui::BeginCombo("KeyName", setKeyName.c_str()))
	{
		for (auto& key : attackKeys_)
		{
			
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (setKeyName == key.first);
			if (ImGui::Selectable(key.first.c_str(), is_selected))
			{
				setKeyName = key.first.c_str();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	static std::string setComboName;		//読み込むファイルの名前
	//プルダウンメニューでコンボを選択
	if (ImGui::BeginCombo("AttackName", setComboName.c_str()))
	{
		for (uint32_t i = 0; i < allAttackFileNames.size(); ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (setComboName == allAttackFileNames[i]);
			if (ImGui::Selectable(allAttackFileNames[i].c_str(), is_selected))
			{
				setComboName = allAttackFileNames[i].c_str();
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Set", ImVec2(50, 50))) {
		attackKeys_.at(setKeyName) = setComboName;
	}
	ImGui::SameLine();
	if (ImGui::Button("Save", ImVec2(50, 50))) {
		AttackKeySave();
	}
	//表示
	ImGui::Text("KeyName : AttackName");
	for (auto& keyName : attackKeys_) {
		std::string text = keyName.first + " : " + keyName.second;
		ImGui::Text(text.c_str());
	}

	ImGui::End();
}

void AttackEditor::ImGuiQuaternion()
{
	ImGui::Begin("Quaternion");

	if (ImGui::Button("Add", ImVec2(50, 50)))
	{
		quaternions_[currentSwingNum_].emplace_back();
		auto& q = quaternions_[currentSwingNum_].back();
		q.q = IdentityQuaternion();
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete", ImVec2(50, 50)))
	{
		if (quaternions_[currentSwingNum_].size() > 1)
		{
			quaternions_[currentSwingNum_].erase(quaternions_[currentSwingNum_].begin() + (quaternions_[currentSwingNum_].size() - 1));
		}
	}
	if (ImGui::CollapsingHeader("Quaternion"))
	{
		std::string IndexNumString;
		std::string frameString;
		std::string quaternionString;
		uint32_t index = 0;

		Quaternion preQuaternion;

		for (auto& q : quaternions_[currentSwingNum_])
		{
			std::ostringstream num;

			num << index;
			IndexNumString = "Num : " + num.str();
			frameString = "Frame" + num.str();
			quaternionString = "Quaternion" + num.str();

			float maxTime = attackInfo_[currentSwingNum_].attackFrame + attackInfo_[currentSwingNum_].gapFrame;
			ImGui::Text(IndexNumString.c_str());
			ImGui::DragFloat(frameString.c_str(), &q.frame, 1.f, 0, maxTime);

			preQuaternion = q.q;

			float quaternion[4] = { q.q.x, q.q.y, q.q.z, q.q.w };
			ImGui::DragFloat4(quaternionString.c_str(), quaternion, 0.01f,-3.14f, 3.14f);
			q.q.x = quaternion[0];
			q.q.y = quaternion[1];
			q.q.z = quaternion[2];
			q.q.w = quaternion[3];

			if ((preQuaternion == q.q) == false)
			{
				cullentQuaternion_ = q.q;
			}

			index++;
		}
	}
	ImGui::End();
}

void AttackEditor::AttackSave(const std::string& string)
{
	std::string saveDir = attackInfoDir_;
	saveDir.append(string.c_str());
	saveDir += ".csv";
	std::ofstream writing_file;

	writing_file.open(saveDir, std::ios::out);

	uint16_t index = 0;

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
		writing_text = "KnockYVec";
		writing_file << writing_text << " = " << attackinfo.knockYVec << std::endl;
		Vector3& addVec = attackinfo.playerMoveVec;
		writing_text = "addVec";
		writing_file << writing_text << " " << addVec.x << " " << addVec.y << " " << addVec.z << std::endl;
		writing_text = "deceleration";
		writing_file << writing_text << " = " << attackinfo.deceleration << std::endl;
		writing_text = "attackType";
		std::string attackType;
		if (attackinfo.attackType == AttackType::Normal)attackType = "Normal";
		else attackType = "Finish";
		writing_file << writing_text << " " << attackType << std::endl;
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
			easingType = configCommon_.GetEasingType();
			writing_file << writing_text << easingType << std::endl;

			writing_text = "EasingTypeInOut ";
			easingType = configCommon_.GetEasingInOutType();
			writing_file << writing_text << easingType << std::endl;
		}

		//スプライトの座標を出力する
		for (auto& spline : splinePointPos_[index])
		{
			Vector3& pos = *spline;
			writing_text = "SplinePos";
			writing_file << writing_text << " " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		}
		
		writing_file << std::endl;
		writing_text = "//--Quaternion--//";
		writing_file << writing_text << std::endl;
		for (auto& q : quaternions_[index])
		{
			writing_text = "flame";
			writing_file << writing_text << " " << q.frame << std::endl;
			writing_text = "Quaternion";
			writing_file << writing_text << " " << q.q.x << " " << q.q.y << " " << q.q.z << " " << q.q.w <<  std::endl;
		}
		index++;
		writing_file << std::endl;
	}
	writing_file.close();
	//現在存在しているファイルを全て検索する
	FindAttackFile();
}

void AttackEditor::AttackLoad(const std::string& string)
{
	splinePointPos_.clear();
	quaternions_.clear();
	attackInfo_.clear();
	std::string loadDir = attackInfoDir_;
	loadDir.append(string.c_str());
	loadDir += ".csv";

	std::ifstream file(loadDir);  // 読み込むファイルのパスを指定
	std::string line;

	AttackInfo* attackinfo = nullptr;
	std::vector<std::unique_ptr<Vector3>>* splinePos = nullptr;
	std::vector<QuaternionControl>* quaternionColtrol = nullptr;
	currentSwingNum_ = -1;
	int32_t quaternionIndex = -1;

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
		else if (key == "KnockYVec")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackinfo->knockYVec;
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
		else if (key == "attackType")
		{
			std::string attackType;
			line_stream >> attackType;

			AttackType timerType;
			if (attackType == "Normal")timerType = AttackType::Normal;
			else timerType = AttackType::Finish;
			attackinfo->attackType = timerType;
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
			std::string easingType;
			line_stream >> easingType;
			configCommon_.SetEasingTypeName(easingType);

			for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
			{
				if (easingType == Spline::GetEaseTypeNames()[i])
				{
					attackinfo->easingType = ((Spline::EasingType)i);
					break;
				}
			}
		}
		//イージング・インアウト読み込み
		if (key == "EasingTypeInOut")
		{
			std::string easingTypeInOut;
			Spline::EasingTypeInOut typeInout;
			line_stream >> easingTypeInOut;
			configCommon_.SetEasingTypeInOutName(easingTypeInOut);

			if (easingTypeInOut == "In")typeInout = Spline::EasingTypeInOut::In;
			else if (easingTypeInOut == "Out")typeInout = Spline::EasingTypeInOut::Out;
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

			ImGuiADDSplinePos(pos, currentSwingNum_);
		}

		if (key == "//--Quaternion--//")
		{
			quaternions_.emplace_back();
			quaternionColtrol = &quaternions_.back();
			quaternionIndex++;
		}
		if (quaternionColtrol == nullptr) continue;
		//スプライン曲線読み込み
		if (key == "flame")
		{
			quaternionColtrol->emplace_back();
			float frame;
			line_stream >> frame;

			quaternionColtrol->back().frame = frame;
		}
		if (key == "Quaternion")
		{
			Quaternion q{};
			line_stream >> q.x;
			line_stream >> q.y;
			line_stream >> q.z;
			line_stream >> q.w;

			quaternionColtrol->back().q = q;
		}


	}
	currentSwingNum_ = 0;
	SetSplinePos();
}

void AttackEditor::FindAttackFile()
{
	std::string dir = attackInfoDir_;
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

	playerObj_->WT_.SetQuaternion(IdentityQuaternion());
	playerObj_->Update();
	currentquaternion_ = 0;

	float time = (quaternions_[currentSwingNum_].at(currentquaternion_).frame - timer_.GetTimer());
	slerpSpeed_ = 1.f / time;
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
			spline_.AddPosition(*splinePointPos_[currentSwingNum_][i], PosState::Start);
		}
		else if (i == splinePointPos_[currentSwingNum_].size() - 1)
		{
			spline_.AddPosition(*splinePointPos_[currentSwingNum_][i], PosState::End);
		}
		else
		{
			spline_.AddPosition(*splinePointPos_[currentSwingNum_][i], PosState::Middle);
		}
	}
}

void AttackEditor::AttackKeySave()
{
	std::string saveDir = attackKeyDir_;
	saveDir += ".csv";
	std::ofstream writing_file;

	writing_file.open(saveDir, std::ios::out);


	for (auto& keys : attackKeys_) {
		writing_file << keys.first << " " << keys.second << std::endl;
	}
	writing_file.close();
}

void AttackEditor::AttackKeyLoad()
{
	std::string loadDir = attackKeyDir_;
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

void AttackEditor::QuaternionUpdate()
{
	Quaternion result = displayPlayerObj_->WT_.quaternion_;
	if (timer_.GetTimer() > quaternions_[currentSwingNum_].at(currentquaternion_).frame)
	{
		if (currentquaternion_ < quaternions_[currentSwingNum_].size() - 1)
		{
			currentquaternion_++;

			float time = (quaternions_[currentSwingNum_].at(currentquaternion_).frame - timer_.GetTimer());
			slerpSpeed_ = 1.f / time;
		}
	}
	auto& currentQuaternion = quaternions_[currentSwingNum_].at(currentquaternion_).q;

	result = result.Slerp(currentQuaternion, slerpSpeed_);

	displayPlayerObj_->WT_.SetQuaternion(result);
}
