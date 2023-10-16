#include "AttackEditor.h"

AttackEditor::AttackEditor()
{
	playerObj_ = std::make_unique<Object3d>();
	playerObj_->SetModel(Model::CreateOBJ_uniptr("player", true));
	
	splineObj_ = std::make_unique<Object3d>();
	splineObj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
	splineObj_->SetAmbient("cube", Vector3(0.5f,0.5f,0));
	splineObj_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
	splineObj_->WT_.parent_ = &playerObj_->WT_;
	isPlay_ = false;
	isPointErase_ = false;
}

void AttackEditor::Update()
{
	if (isPointErase_) {
		isPointErase_ = false;
		splinePointPos_.erase(splinePointPos_.begin() + (splinePointPos_.size() - 1));
	}
	for (int32_t i = 0; i < splinePointPos_.size();i++) {
		splinePointPos_[i]->obj_->SetPos(splinePointPos_[i]->splinePointPos_);
		splinePointPos_[i]->obj_->Update();
	}
	if (isPlay_) {
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
	if (isPlay_) {
		splineObj_->Draw();
	}
	for (auto& spline : splinePointPos_) {
		spline->obj_->Draw();
	}
}

void AttackEditor::DrawImGui()
{
	//プレイヤー座標の変更を行う
	ImGui::Begin("AttackEditor");

	static float playerpos[3] = {0,1,0};

	ImGui::DragFloat3("pos", playerpos,0.1f,100.f,100.f);
	playerObj_->SetPos(Vector3(playerpos[0], playerpos[1], playerpos[2]));

	if (ImGui::Button("PlayerPosReset")) {
		playerObj_->SetPos(Vector3(0,1,0));
	}

	ImGui::End();
	//スプラインポイントの変更・スプラインポイントの追加
	ImGui::Begin("Spline");
	if (ImGui::Button("Play",ImVec2(50, 50))) {
		if (splinePointPos_.size() > 1) {
			isPlay_ = true;
			spline_.SetIsStart(true);
			spline_.AllClear();
			spline_.SetMaxTime(attackInfo_.attackFrame);
			for (int32_t i = 0; i < splinePointPos_.size(); i++) {

				if (i == 0) {
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::Start);
				}
				else if (i == splinePointPos_.size() - 1) {
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::End);
				}
				else {
					spline_.AddPosition(splinePointPos_[i]->splinePointPos_, PosState::Middle);
				}
			}

		}
	}
	if (ImGui::Button("AddSplinePoint")) {
		std::unique_ptr<SplinePos> newObj = std::make_unique<SplinePos>();
		newObj->obj_ = std::make_unique<Object3d>();
		newObj->obj_->SetModel(Model::CreateOBJ_uniptr("cube", true));
		newObj->obj_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
		newObj->obj_->WT_.parent_ = &playerObj_->WT_;
		splinePointPos_.emplace_back(std::move(newObj));
	}
	if (ImGui::Button("DeleteSplinePoint")) {
		if (splinePointPos_.size() > 0) {
			isPointErase_ = true;
		}
	}
	if (ImGui::Button("SplineTimerType")) {
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
		for (auto& spline : splinePointPos_) {
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

	ImGui::DragFloat("AttackTime", &attackInfo_.attackFrame, 1.0f, 0.f, 500.f);

	ImGui::End();
}

void AttackEditor::ImGuiSetEasingType()
{
	std::vector<std::string> easingTypenames;
	easingTypenames.emplace_back("Back");
	easingTypenames.emplace_back("Bounce");
	easingTypenames.emplace_back("Circ");
	easingTypenames.emplace_back("Quint");
	easingTypenames.emplace_back("Cubic");
	easingTypenames.emplace_back("Sine");
	//プルダウンメニューで読み込んだファイルを選択できるようにする
	static std::string s_currentItem = "Back";
	if (ImGui::BeginCombo("EasingType", s_currentItem.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingType::EasingTypeEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (s_currentItem == easingTypenames[i]);
			if (ImGui::Selectable(easingTypenames[i].c_str(), is_selected))
			{
				s_currentItem = easingTypenames[i].c_str();
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
	static std::string s_currentItem = "In";
	if (ImGui::BeginCombo("InOut", s_currentItem.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingTypeInOut::EasingTypeInOutEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (s_currentItem == easingTypenames[i]);
			if (ImGui::Selectable(easingTypenames[i].c_str(), is_selected))
			{
				s_currentItem = easingTypenames[i].c_str();
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
