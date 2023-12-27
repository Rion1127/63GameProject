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
		float cameraShakePower;
		float cameraShakeTime;
	};
private:
	std::vector<EffectInfo> effectInfo_;
public:
	AttackEffectEditor();
	//更新
	void Update();
	//描画
	void Draw();
	//ImGui描画
	void DrawImGui();
private:
	//演出に関する項目をImGuiで変更する
	void ImGuiInfoEdit();
};

