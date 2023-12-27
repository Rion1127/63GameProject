#include "AttackEditorConfigCommon.h"
#include <imgui.h>

/**
 * @file AttackEditorConfigCommon.cpp
 * @brief アタックエディターに共通する設定をまとめたクラス
 */

void AttackEditorConfigCommon::SetTimerType(Spline::TimerType& timerType)
{
	if (ImGui::Button("SplineTimerType"))
	{
		bool flag = (timerType == Spline::TimerType::Easing);

		timerType = flag ? Spline::TimerType::Normal : Spline::TimerType::Easing;
	}
	std::string timerTypeString;
	if (timerType == Spline::TimerType::Easing) timerTypeString = "Easing";
	else timerTypeString = "Normal";
	ImGui::SameLine();
	ImGui::Text(timerTypeString.c_str());
}

void AttackEditorConfigCommon::SetEasingType(Spline::EasingType& easingType)
{
	std::vector<std::string> easingTypeNames;
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
				easingType = ((Spline::EasingType)i);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void AttackEditorConfigCommon::SetEasingTypeInOut(Spline::EasingTypeInOut& timerTypeInOut)
{
	std::vector<std::string> easingTypenames;
	easingTypenames.emplace_back("In");
	easingTypenames.emplace_back("Out");
	easingTypenames.emplace_back("InOut");
	//プルダウンメニューでイージングタイプインアウトを選択できるようにする
	if (ImGui::BeginCombo("InOut", easingInOutType_.c_str()))
	{
		for (uint32_t i = 0; i < (uint32_t)Spline::EasingTypeInOut::EasingTypeInOutEnd; ++i)
		{
			//選択したものとハッシュ値が一致したらs_currentItemにハッシュ値を代入
			const bool is_selected = (easingInOutType_ == easingTypenames[i]);
			if (ImGui::Selectable(easingTypenames[i].c_str(), is_selected))
			{
				easingInOutType_ = easingTypenames[i].c_str();
				timerTypeInOut = ((Spline::EasingTypeInOut)i);
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
