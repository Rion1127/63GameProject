#pragma once
#include "Object3d.h"
#include "Spline.h"
#include "IAttack.h"
#include "mInput.h"
#include "Sword.h"
#include "Timer.h"
#include "PostureDisplay.h"
#include <ImGui/imgui.h>
#include <vector>
#include <map>

/**
 * @file AttackEditor.h
 * @brief 攻撃を制作するエディタークラス
 */

enum class ColType {
	Normal,		//剣と当たり判定が一緒
	Separate	//剣と当たり判定が別別に動く
};

class AttackEditor
{
public:
	struct QuaternionControl {
		float frame;
		Quaternion q;
	};
	//一振り分の攻撃
	struct AttackInfo {
		float attackAllFrame;	//一振りにかかる時間(後隙含む)
		float attackFrame = 10;	//攻撃に掛かるフレーム
		float gapFrame;			//後隙フレーム

		Vector3 gravity;		//攻撃時に動く方向
		int32_t damage;			//ダメージ

		Vector3 knockVec;		//ノックバック方向
		float knockYVec;
		Vector3 playerMoveVec;		//プレイヤーが動く方向
		float deceleration = 0.1f;			//プレイヤーの速度減速値

		Spline::TimerType timerType;		//通常かイージングか
		Spline::EasingType easingType;		//イージングの種類
		Spline::EasingTypeInOut inOutType;	//イージングの動き方

		std::vector<Vector3> splinePos;
		std::vector<QuaternionControl> quaternion;
		AttackType attackType = AttackType::Normal;
	};
	struct SplinePos {
		Vector3 splinePointPos_;
	};
	
private:
	//一振り分の詳細
	std::vector<AttackInfo> attackInfo_;
	//プレイヤー
	std::unique_ptr<Object3d> playerObj_;
	std::unique_ptr<Object3d> displayPlayerObj_;
	//剣
	std::unique_ptr<Sword> swordObj_;
	//スプラインの制御点
	std::vector<std::vector<std::unique_ptr<SplinePos>>> splinePointPos_;
	//姿勢制御
	std::vector<std::vector<QuaternionControl>> quaternions_;
	//イージングタイプ
	std::vector<std::string> easingTypeNames;
	//現在選択しているスプライン曲線
	Spline spline_;
	std::string easingType_;
	std::string easingTypeInOut_;
	//選択している一振りを再生
	bool isPlay_;
	//選択している攻撃すべて
	bool isAllPlay_;
	//制御点を消す
	bool isPointErase_;
	//制御点の座標を更新したフラグ
	bool isValueChange_;
	//現在選択している一振り
	int32_t currentSwingNum_;
	int32_t currentquaternion_;
	float slerpSpeed_;
	//プレイヤーが動くベクトル
	Vector3 moveVec_;
	//攻撃時間
	TimerFloat timer_;
	//読み込んだファイルの名前の配列
	std::vector<std::string> allAttackFileNames;
	std::vector<std::string> allAttackKeyNames;

	std::map<std::string, std::string> attackKeys_;
	//保存するファイルのディレクトリ名
	std::string attackInfoDir_;
	std::string attackKeyDir_;
	std::string loadFileName_;
	//クォータニオンの姿勢確認
	PostureDisplay postureDisplay;
	Quaternion cullentQuaternion_;
	//当たり判定の種類
	ColType colType_;
	Spline colSpline_;
public:
	AttackEditor();
	void Update();
	void Draw();
	void DrawRenderTexture();
	void DrawImGui();
private:
	void ImGuiDisplaySplitePoint();	//スプライン曲線の制御点を描画
	void ImGuiSetEasingType();		//イージングのタイプを選択する
	void ImGuiSetEasingTypeInOut();	//イージングのタイプ(InOut)を選択する
	void ImGuiADDSplinePos(const Vector3& pos = {0,0,0},uint32_t index = 0);	//イージングのタイプ(InOut)を選択する
	void ImGuiSave();				//セーブImGui表示
	void ImGuiLoad();				//ロードImGui表示
	void ImGuiPlay();				//セーブImGui表示
	void ImGuiAllPlay();			//オールプレイImGui表示
	void ImGuiSplineEditor();		//剣の軌道
	void ImGuiAttackInfo();			//攻撃のステータス
	void ImGuiSwingCount();			//多段攻撃
	void ImGuiSettingCombo();		//攻撃組み替え
	void ImGuiQuaternion();			//姿勢制御
	void ImGuiColSpline();			//当たり判定の動き
	void AttackSave(const std::string& string);
	void AttackLoad(const std::string& string);
	void FindAttackFile();		//ディレクトリ内にある攻撃ファイルを読み込む
	void AttackPlay();
	void SetSplinePos();
	void AttackKeySave();
	void AttackKeyLoad();
	void QuaternionUpdate();
};