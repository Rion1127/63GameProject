#pragma once
#include "AttackEditorConfigCommon.h"
#include "Spline.h"
#include "Object3d.h"
#include <vector>

class ColSpline {
public:
	struct ColScale
	{
		float frame;
		float scale;
	};
	struct SeparateAttackInfo {
		float attackFrame = 10;	//攻撃に掛かるフレーム
		int32_t damage;			//ダメージ
		float radian;			//半径

		Spline::TimerType timerType;		//通常かイージングか
		Spline::EasingType easingType;		//イージングの種類
		Spline::EasingTypeInOut inOutType;	//イージングの動き方
		std::vector<Vector3> splinePos;
		std::vector<ColScale> colScales;
	};
private:
	std::unique_ptr<Object3d> colObj_;
	SeparateAttackInfo attackInfo_;
	Spline spline_;
	int32_t currentNum_;
	int32_t currentColScaleNum_;


	AttackEditorConfigCommon configCommon_;

	bool isErase_;
	bool isPosChange_;
	
	Vector3 prePos_;
public:
	ColSpline();

	void Update();
	void Draw();
	void DrawImGui();
private:
	void ImGuiColSplineEditor();
	void ImGuiDisplaySplinePos();
	void ImGuiColInfo();
	void ImGuiColScaleTransition();

	void SetSplinePoint();
public:
	void PlaySpline();
};

