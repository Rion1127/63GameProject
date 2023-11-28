#include "AttacColSpline.h"
#include <imgui.h>

ColSpline::ColSpline()
{
	attackInfo_.emplace_back();
	spline_.emplace_back();
}

void ColSpline::Update()
{
}

void ColSpline::Draw()
{
}

void ColSpline::ImGuiAddSplinePoint()
{
	ImGui::Begin("ColSplinePoint");

	if (ImGui::Button("AddPoint"))
	{
		attackInfo_[currentNum_].splinePos.emplace_back();
		
	}


	ImGui::End();
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
