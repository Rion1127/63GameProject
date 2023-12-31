#pragma once
#include "Object3d.h"
#include "Spline.h"
#include "IAttack.h"
#include "mInput.h"
#include "Sword.h"
#include "Timer.h"
#include "PostureDisplay.h"
#include "AttackColEditor.h"
#include "AttackEffectEditor.h"
#include "AttackEditorConfigCommon.h"
#include "DebugActor.h"
#include <ImGui/imgui.h>
#include <vector>
#include <map>

/**
 * @file AttackEditor.h
 * @brief 攻撃を制作するエディタークラス
 */

enum class ColType {
	Normal,
	Separate
};

class AttackEditor
{
public:
	struct QuaternionControl {
		float frame;
		Quaternion q = IdentityQuaternion();
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

		std::vector<Vector3> splinePos;		//剣の軌道スプラインの座標
		std::vector<QuaternionControl> quaternion;	//プレイヤーの姿勢
		AttackType attackType = AttackType::Normal;	//攻撃のタイプ通常かフィニッシュ
		ColSpline::SeparateAttackInfo colInfo;		//当たり判定の詳細
		ColType colType_ = ColType::Normal;			//当たり判定の種類
		AttackEffectEditor::EffectInfo effectInfo;	//演出関連
	};
private:
	//一振り分の詳細
	std::vector<AttackInfo> attackInfo_;
	//プレイヤー
	DebugActor debugAcrot_;
	Gravity gravity_;
	//剣
	std::unique_ptr<Sword> swordObj_;
	//スプラインの制御点
	std::vector<std::vector<std::unique_ptr<Vector3>>> splinePointPos_;
	//姿勢制御
	std::vector<std::vector<QuaternionControl>> quaternions_;
	AttackEditorConfigCommon configCommon_;
	//現在選択しているスプライン曲線
	Spline spline_;
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
	//現在選択している当たり判定
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

	ColSpline colSpline_;
	//当たり判定の種類
	ColType colType_;

	AttackEffectEditor effectEditor_;
public:
	AttackEditor();
	void Update();	//更新
	void Draw();	//モデルを描画する
	void DrawRenderTexture();	//プレビューのモデルを描画する
	void DrawImGui();//ImGuiを描画する
private:
	void ImGuiDisplaySplitePoint();	//スプライン曲線の制御点を描画
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
	void AttackSave(const std::string& string);
	void AttackLoad(const std::string& string);
	void FindAttackFile();		//ディレクトリ内にある攻撃ファイルを読み込む
	void AttackPlay();			//攻撃の動きを再生する
	void SetSplinePos();		//スプライン曲線の制御点をセットする
	void AttackKeySave();		//攻撃のコンボに使うキーを保存する
	void AttackKeyLoad();		//攻撃のコンボに使うキーを読み込む
	void QuaternionUpdate();	//プレビューの姿勢を更新する
};