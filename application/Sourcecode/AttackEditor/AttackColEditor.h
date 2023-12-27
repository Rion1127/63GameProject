#pragma once
#include "AttackEditorConfigCommon.h"
#include "Spline.h"
#include "Object3d.h"
#include <vector>

/**
 * @file AttackColEditor.h
 * @brief アタックエディターの当たり判定に関するクラス
 */

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
	float colRadius_;

	AttackEditorConfigCommon configCommon_;

	bool isErase_;
	bool isPosChange_;
	
	Vector3 prePos_;
public:
	ColSpline();
	//更新
	void Update();
	//描画
	void Draw();
	//ImGui描画
	void DrawImGui();
private:
	//ImGuiでスプライン関連の数値を変更する
	void ImGuiColSplineEditor();
	//ImGuiでスプラインの制御点を変更する
	void ImGuiDisplaySplinePos();
	//ImGuiで当たり判定の詳細を変更する
	void ImGuiColInfo();
	//スプライン曲線の制御点をセットする
	void SetSplinePoint();
public:
	//スプライン曲線の動きを再生する
	void PlaySpline();
	//ファイル出力
	void OutPutStatus(std::ofstream& writing_file);
	//ファイル読み込み
	void InPutStatus(const std::string& key,std::stringstream& line_stream);
public:
	SeparateAttackInfo GetAttackInfo() { return attackInfo_; }
};

