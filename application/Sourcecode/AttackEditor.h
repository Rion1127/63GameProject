#pragma once
#include "Object3d.h"
#include "Spline.h"
#include "IAttack.h"
#include "mInput.h"
#include <imgui.h>
#include <vector>

/**
 * @file AttackEditor.h
 * @brief 攻撃を制作するエディタークラス
 */

class AttackEditor
{
public:
	//一振り分の攻撃
	struct AttackInfo {
		float attackAllFrame;	//一振りにかかる時間(後隙含む)
		float attackFrame = 10;	//攻撃に掛かるフレーム
		float gapFrame;			//後隙フレーム

		Vector3 gravity;		//攻撃時に動く方向
		int32_t damage;			//ダメージ

		Vector3 knockVec;		//ノックバック方向
		Vector3 playerMoveVec;		//プレイヤーが動く方向
		float deceleration;			//プレイヤーの速度減速値

		Spline::TimerType timerType;		//通常かイージングか
		Spline::EasingType easingType;		//イージングの種類
		Spline::EasingTypeInOut inOutType;	//イージングの動き方

		std::vector<Vector3> splinePos;
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

	Vector3 moveVec_;

	std::vector<std::string> allAttackFileNames;
public:
	AttackEditor();
	void Update();
	void Draw();
	void DrawImGui();
private:
	void ImGuiSetEasingType();	//イージングのタイプを選択する
	void ImGuiSetEasingTypeInOut();	//イージングのタイプ(InOut)を選択する
	void ImGuiADDSplinePos(const Vector3& pos = {0,0,0});	//イージングのタイプ(InOut)を選択する
	void ImGuiSave();
	void ImGuiLoad();
	void AttackSave(const std::string& string);
	void AttackLoad(const std::string& string);
	void FindAttackFile();		//ディレクトリ内にある攻撃ファイルを読み込む
};

class FrameEditor {
private:

public:

};