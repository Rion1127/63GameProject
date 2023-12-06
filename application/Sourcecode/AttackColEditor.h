#pragma once
#include "AttackEditorConfigCommon.h"
#include "Spline.h"
#include "Object3d.h"
#include <vector>

class ColSpline {
public:
	struct SeparateAttackInfo {
		float activeFrame;
		float attackFrame = 10;	//攻撃に掛かるフレーム
		int32_t damage;			//ダメージ
		float firstRadius;			//半径
		float addRadiusValue;		//半径が変化していく速度

		Spline::TimerType timerType;		//通常かイージングか
		Spline::EasingType easingType;		//イージングの種類
		Spline::EasingTypeInOut inOutType;	//イージングの動き方
		std::vector<Vector3> splinePos;
	};
private:
	std::unique_ptr<Object3d> colObj_;
	SeparateAttackInfo attackInfo_;
	Spline spline_;
	int32_t currentNum_;
	int32_t currentColScaleNum_;
	float colRadius_;

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
	
	void SetSplinePoint();
public:
	void PlaySpline();
	//ファイル出力
	void OutPutStatus(std::ofstream& writing_file);
	//ファイル読み込み
	void InPutStatus(const std::string& key,std::stringstream& line_stream);
public:
	SeparateAttackInfo GetAttackInfo() { return attackInfo_; }
};

