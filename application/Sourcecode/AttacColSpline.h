#pragma once
#include "Spline.h"
#include <vector>

class ColSpline {
private:
	struct SeparateAttackInfo {
		float attackFrame = 10;	//攻撃に掛かるフレーム
		int32_t damage;			//ダメージ
		float radian;			//半径

		Spline::TimerType timerType;		//通常かイージングか
		Spline::EasingType easingType;		//イージングの種類
		Spline::EasingTypeInOut inOutType;	//イージングの動き方
		std::vector<Vector3> splinePos;
	};
private:
	std::vector<SeparateAttackInfo> attackInfo_;
	std::vector<Spline> spline_;
	int32_t currentNum_;
public:
	ColSpline();

	void Update();
	void Draw();
private:
	void ImGuiAddSplinePoint();
	void SetSplinePoint();
};

