#include "AttackEditor.h"
#include <fstream>
#include <iostream>

AttackEditor::AttackEditor()
{
	playerObj_ = std::make_unique<Object3d>();
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true));

	splineObj_ = std::make_unique<Object3d>();
	splineObj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	splineObj_->SetAmbient("cube", Vector3(0.5f, 0.5f, 0));
	splineObj_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	splineObj_->WT_.parent_ = &playerObj_->WT_;
	isPlay_ = false;
	isPointErase_ = false;
}

void AttackEditor::Update()
{
	if (isPointErase_)
	{
		isPointErase_ = false;
		splinePointPos_.erase(splinePointPos_.begin() + (splinePointPos_.size() - 1));
	}
	for (int32_t i = 0; i < splinePointPos_.size(); i++)
	{
		splinePointPos_[i]->obj_->SetPos(splinePointPos_[i]->splinePointPos_);
		splinePointPos_[i]->obj_->Update();
	}
	if (isPlay_)
	{
		spline_.Update();

		splineObj_->SetPos(spline_.GetNowPoint());

		if (spline_.GetisEnd())isPlay_ = false;
	}
	splineObj_->Update();
	playerObj_->Update();
}

void AttackEditor::Draw()
{
	playerObj_->Draw();

	splineObj_->Draw();

	for (auto& spline : splinePointPos_)
	{
		spline->obj_->Draw();
	}
}

void AttackEditor::DrawImGui()
{
	//プレイヤー座標の変更を行う
	ImGui::Begin("AttackEditor");

	static float playerpos[3] = { 0,1,0 };

	ImGui::DragFloat3("pos", playerpos, 0.1f, 100.f, 100.f);
	playerObj_->SetPos(Vector3(playerpos[0], playerpos[1], playerpos[2]));

	if (ImGui::Button("PlayerPosReset"))
	{
		playerpos[0] = 0;
		playerpos[1] = 1;
		playerpos[2] = 0;
	}

	ImGuiSave();
	ImGuiLoad();

	ImGui::End();
	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackSpline");
	if (ImGui::Button("Play", ImVec2(50, 50)))
	{
		if (splinePointPos_.size() > 1)
		{
			isPlay_ = true;
			spline_.SetIsStart(true);
			spline_.AllClear();
			spline_.SetMaxTime(attackInfo_.attackFrame);
			for (int32_t i = 0; i < splinePointPos_.size(); i++)
			{
				if (i == 0)
				{
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::Start);
				}
				else if (i == splinePointPos_.size() - 1)
				{
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::End);
				}
				else
				{
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::Middle);
				}
			}

		}
	}
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
		bool flag = (spline_.GetTimerType() == Spline::TimerType::Easing);

		timerType = flag ? Spline::TimerType::Normal : Spline::TimerType::Easing;

		spline_.SetTimerType_(timerType);
	}
	std::string timerType;
	if (spline_.GetTimerType() == Spline::TimerType::Easing) timerType = "Easing";
	else timerType = "Normal";
	ImGui::SameLine();
	ImGui::Text(timerType.c_str());

	ImGuiSetEasingType();
	ImGuiSetEasingTypeInOut();

	if (ImGui::CollapsingHeader("SplinePointPosision"))
	{
		int32_t splinePosIndex = 0;
		std::string splinePosName;
		for (auto& spline : splinePointPos_)
		{
			std::ostringstream num;

			num << splinePosIndex;
			splinePosName = "Sprite" + num.str();

			float splinePos[3] = {
				spline->splinePointPos_.x,
				spline->splinePointPos_.y,
				spline->splinePointPos_.z,
			};
			ImGui::DragFloat3(splinePosName.c_str(), splinePos, 0.1f, 100.f, 100.f);

			spline->splinePointPos_ = { splinePos[0], splinePos[1],  splinePos[2] };

			splinePosIndex++;
		}
	}
	ImGui::End();

	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("AttackInfo");

	ImGui::DragFloat("attackFrame", &attackInfo_.attackFrame, 1.0f, 0.f, 500.f);
	ImGui::DragFloat("gapFrame", &attackInfo_.gapFrame, 1.0f, 0.f, 500.f);
	ImGui::DragFloat("Damage", &attackInfo_.damage, 1.0f, 0.f, 500.f);
	ImGui::DragFloat("gravity", &attackInfo_.gravity.y, 1.0f, 0.f, 500.f);

	ImGui::End();
}

void AttackEditor::ImGuiSetEasingType()
{
	easingTypeNames.emplace_back("Back");
	easingTypeNames.emplace_back("Bounce");
	easingTypeNames.emplace_back("Circ");
	easingTypeNames.emplace_back("Quint");
	easingTypeNames.emplace_back("Cubic");
	easingTypeNames.emplace_back("Sine");
	//プルダウンメニューで読み込んだファイルを選択できるようにする
	if (ImGui::BeginCombo("EasingType", easingType_.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (easingType_ == easingTypeNames[i]);
			if (ImGui::Selectable(easingTypeNames[i].c_str(), is_selected))
			{
				easingType_ = easingTypeNames[i].c_str();
				spline_.SetEasingType_((Spline::EasingType)i);
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
	//プルダウンメニューで読み込んだファイルを選択できるようにする
	if (ImGui::BeginCombo("InOut", easingTypeInOut_.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingTypeInOut::EasingTypeInOutEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (easingTypeInOut_ == easingTypenames[i]);
			if (ImGui::Selectable(easingTypenames[i].c_str(), is_selected))
			{
				easingTypeInOut_ = easingTypenames[i].c_str();
				spline_.SetEasingTypeInOut_((Spline::EasingTypeInOut)i);
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
	newObj->obj_ = std::make_unique<Object3d>();
	newObj->obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	newObj->obj_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	newObj->obj_->WT_.parent_ = &playerObj_->WT_;
	newObj->splinePointPos_ = pos;
	splinePointPos_.emplace_back(std::move(newObj));
}

void AttackEditor::ImGuiSave()
{
	static std::string saveName;		//書き出すファイルの名前
	static bool isProofSave = false;	//セーブの確認
	ImGui::InputText("SaveName", saveName.data(), 10);

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

	ImGui::InputText("LoadName", LoadName.data(), 10);
	
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

void AttackEditor::AttackSave(const std::string& string)
{
	std::string saveDir = "application/Resources/AttackInfo/";
	saveDir.append(string.c_str());
	saveDir += ".csv";
	std::ofstream writing_file;

	writing_file.open(saveDir, std::ios::out);

	std::string writing_text = "//--AtatckInfo--//";
	writing_file << writing_text << std::endl;
	writing_text = "attackFrame";
	writing_file << writing_text << " = " << attackInfo_.attackFrame << std::endl;
	writing_text = "gapFrame";
	writing_file << writing_text << " = " << attackInfo_.gapFrame << std::endl;
	writing_text = "damege";
	writing_file << writing_text << " = " << attackInfo_.damage << std::endl;
	writing_text = "gravityY";
	writing_file << writing_text << " = " << attackInfo_.gravity.y << std::endl;
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
	for (auto& spline : splinePointPos_)
	{
		Vector3& pos = spline->splinePointPos_;
		writing_text = "SplinePos";
		writing_file << writing_text << " " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}

	writing_file.close();
}

void AttackEditor::AttackLoad(const std::string& string)
{
	splinePointPos_.clear();
	std::string saveDir = "application/Resources/AttackInfo/";
	saveDir.append(string.c_str());
	saveDir += ".csv";

	std::ifstream file(saveDir);  // 読み込むファイルのパスを指定
	std::string line;

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
			line_stream >> attackInfo_.attackFrame;
		}
		else if (key == "gapFrame")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackInfo_.gapFrame;
		}
		else if (key == "damege")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackInfo_.damage;
		}
		else if (key == "gravityY")
		{
			line_stream.ignore(1, '=');
			line_stream >> attackInfo_.gravity.y;
		}
		//タイマー制御方法読み込み
		if (key == "TimerType")
		{
			std::string timerTypeName;
			line_stream >> timerTypeName;

			Spline::TimerType timerType;
			if (timerTypeName == "Normal")timerType = Spline::TimerType::Normal;
			else timerType = Spline::TimerType::Easing;
			spline_.SetTimerType_(timerType);
		}
		//イージングの種類読み込み
		if (key == "EasingType")
		{
			line_stream >> easingType_;

			for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; i++)
			{
				if (easingType_ == easingTypeNames[i])
				{
					spline_.SetEasingType_((Spline::EasingType)i);
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

			spline_.SetEasingTypeInOut_(typeInout);
		}
		//スプライン曲線読み込み
		if (key == "SplinePos")
		{
			Vector3 splinePos;

			line_stream >> splinePos.x;
			line_stream >> splinePos.y;
			line_stream >> splinePos.z;

			ImGuiADDSplinePos(splinePos);
		}
	}


}
