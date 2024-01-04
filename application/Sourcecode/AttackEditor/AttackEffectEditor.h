#pragma once
#include <vector>
#include <string>

/**
 * @file AttackEffectEditor.h
 * @brief アタックエディターの演出に関連する設定をするクラス
 */

class AttackEffectEditor
{
public:
	struct EffectInfo{
		int32_t frame;

		std::string particleName;
		bool isSeparateParticlePos_;	//パーティクルを剣の軌道に出すか別の座標に出すか
		float cameraShakePower;
		float cameraShakeTime;
	};
private:
	std::vector<EffectInfo> effectInfo_;

	bool isDelete_;
public:
	AttackEffectEditor();
	//更新
	void Update();
	//描画
	void Draw();
	//ImGui描画
	void DrawImGui(EffectInfo& info);
	//ファイル出力
	void OutPutStatus(std::ofstream& writing_file, const EffectInfo& info);
	//ファイル読み込み
	void InPutStatus(const std::string& key, std::stringstream& line_stream, EffectInfo& info);
private:
	//演出に関する項目をImGuiで変更する
	void ImGuiInfoEdit(EffectInfo& info);
	//effectInfo_を追加する
	void ImGuiAddInfo();
	//effectInfo_を削除する
	void ImGuiDeleteInfo();
};

