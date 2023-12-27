#pragma once
#include "Spline.h"
#include <string>

/**
 * @file AttackEditorConfigCommon.h
 * @brief アタックエディターに共通する設定をまとめたクラス
 */

class AttackEditorConfigCommon
{
private:
	std::string timerType_;
	std::string easingType_;
	std::string easingInOutType_;
public:
	//Spline::TimerTypeをセットする
	void SetTimerType(Spline::TimerType& timerType);
	//Spline::EasingTypeをセットする
	void SetEasingType(Spline::EasingType& easingType);
	//Spline::EasingTypeInOutをセットする
	void SetEasingTypeInOut(Spline::EasingTypeInOut& timerTypeInOut);
public:
	std::string GetTimerType() { return timerType_; }
	std::string GetEasingType() { return easingType_; }
	std::string GetEasingInOutType() { return easingInOutType_; }
public:
	void SetTimerTypeName(const std::string& timerType) { timerType_ = timerType; };
	void SetEasingTypeName(const std::string& easingType) { easingType_ = easingType; };
	void SetEasingTypeInOutName(const std::string& timerTypeInOut) { easingInOutType_ = timerTypeInOut; };
};

