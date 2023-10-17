#pragma once
#include "Object3d.h"
#include "Spline.h"
#include "IAttack.h"
#include <imgui.h>
#include <vector>

class AttackEditor
{
private:
	struct AttackInfo {
		float attackAllFrame;	//攻撃全体に掛かるフレーム
		float attackFrame = 10;	//攻撃に掛かるフレーム
		float gapFrame;			//後隙フレーム

		Vector3 gravity;		//攻撃時に動く方向
		float damage;			//ダメージ
	};
	struct AttackOutput {
		AttackType type_;
		Spline colSpline_;
		Spline playerPospline_;
	};
	struct SplinePos {
		std::unique_ptr<Object3d> obj_;
		Vector3 splinePointPos_;
	};
private:
	AttackInfo attackInfo_;

	std::unique_ptr<Object3d> playerObj_;
	std::unique_ptr<Object3d> splineObj_;
	std::vector<std::unique_ptr<SplinePos>> splinePointPos_;

	std::vector<std::string> easingTypeNames;
	Spline spline_;
	std::string easingType_;
	std::string easingTypeInOut_;

	bool isPlay_;
	bool isPointErase_;
public:
	AttackEditor();
	void Update();
	void Draw();
	void DrawImGui();
private:
	void ImGuiSetEasingType();	//イージングのタイプを選択する
	void ImGuiSetEasingTypeInOut();	//イージングのタイプ(InOut)を選択する
	void ImGuiADDSplinePos(const Vector3& pos = {0,0,0});	//イージングのタイプ(InOut)を選択する
	void AttackSave(const std::string& string);
	void AttackLoad(const std::string& string);
};

