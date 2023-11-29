#include "AttackColSpline.h"
#include <imgui.h>

ColSpline::ColSpline()
{
	attackInfo_.emplace_back();
	spline_.emplace_back();
	spline_.back().SetIsLineDisplay(true);
	spline_.back().SetColor(Color(255,0,0,255));
}

void ColSpline::Update()
{
	if (isErase_) {
		isErase_ = false;
		auto& pos = attackInfo_[currentNum_].splinePos;
		if (pos.size() > 0) {
			pos.erase(pos.begin() + (pos.size() - 1));
			SetSplinePoint();
		}
	}
	if (isPosChange_) {
		SetSplinePoint();
	}

	if (spline_[currentNum_].GetsplinePos().size()) {
		spline_[currentNum_].Update();
	}
}

void ColSpline::Draw()
{
	spline_[currentNum_].DrawDebug();
}

void ColSpline::DrawImGui()
{
	ImGui::Begin("ColSplineEditor");
	ImGuiColSplineEditor();
	ImGuiDisplaySplinePos();
	ImGui::End();
}

void ColSpline::ImGuiColSplineEditor()
{
	//制御点追加
	if (ImGui::Button("AddPoint"))
	{
		attackInfo_[currentNum_].splinePos.emplace_back();
		SetSplinePoint();
	}
	//制御点削除
	if (ImGui::Button("DeletePoint"))
	{
		isErase_ = true;
	}
}

void ColSpline::ImGuiDisplaySplinePos()
{
	auto& pos = attackInfo_[currentNum_].splinePos;
	if (ImGui::CollapsingHeader("ColSplinePointPosision"))
	{
		int32_t splinePosIndex = 0;
		std::string splinePosName;
		Vector3 prePos;
		for (auto& spline : pos)
		{
			prePos = spline;
			std::ostringstream num;

			num << splinePosIndex;
			splinePosName = "Sprine" + num.str();

			float splinePos[3] = {
				spline.x,
				spline.y,
				spline.z,
			};
			ImGui::DragFloat3(splinePosName.c_str(), splinePos, 0.1f, 100.f, 100.f, "%.3f");
			spline = { splinePos[0], splinePos[1],  splinePos[2] };
			if (prePos != spline)isPosChange_ = true;
			splinePosIndex++;
		}
	}
}

void ColSpline::SetSplinePoint()
{
	auto& spline = spline_[currentNum_];
	auto& splinePos = attackInfo_[currentNum_].splinePos;
	spline.AllClear();
	spline.SetMaxTime(attackInfo_[currentNum_].attackFrame);
	spline.SetTimerType_(attackInfo_[currentNum_].timerType);
	spline.SetEasingType_(attackInfo_[currentNum_].easingType);
	spline.SetEasingTypeInOut_(attackInfo_[currentNum_].inOutType);
	for (int32_t i = 0; i < attackInfo_[currentNum_].splinePos.size(); i++)
	{
		if (i == 0)
		{
			spline.AddPosition(splinePos[i], PosState::Start);
		}
		else if (i == splinePos.size() - 1)
		{
			spline.AddPosition(splinePos[i], PosState::End);
		}
		else
		{
			spline.AddPosition(splinePos[i], PosState::Middle);
		}
	}
}
