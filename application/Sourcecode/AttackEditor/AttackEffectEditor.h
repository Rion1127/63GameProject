#pragma once
#include <vector>
#include <string>
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
	void Update();
	void Draw();
	void DrawImGui();
private:
	void ImGuiInfoEdit();
};

