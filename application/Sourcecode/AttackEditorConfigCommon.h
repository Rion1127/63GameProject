#pragma once
#include "Spline.h"
#include <string>
class AttackEditorConfigCommon
{
private:
	std::string timerType_;
	std::string easingType_;
	std::string easingInOutType_;
public:
	void SetTimerType(Spline::TimerType& timerType);
	void SetEasingType(Spline::EasingType& easingType);
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

