#include "AttackEffectEditor.h"
#include <imgui.h>
#include "Util.h"

AttackEffectEditor::AttackEffectEditor()
{
	effectInfo_.emplace_back();
}

void AttackEffectEditor::Update()
{
}

void AttackEffectEditor::Draw()
{
}

void AttackEffectEditor::DrawImGui()
{
	ImGui::Begin("EffectEditor");

	ImGuiInfoEdit();

	ImGui::End();
}

void AttackEffectEditor::ImGuiInfoEdit()
{
	int32_t index = 0;
	std::string tagName;
	for (auto& info : effectInfo_) {
		tagName = "EffectInfo" + NumberToString(index);
		if (ImGui::CollapsingHeader(tagName.c_str()))
		{
			ImGui::DragInt("frame", &info.frame);
			ImGui::DragFloat("cameraShakePower", &info.cameraShakePower);
			ImGui::DragFloat("cameraShakeTime", &info.cameraShakeTime);
			ImGui::InputText("cameraShakeTime", info.particleName.data(), 20);
		}
		index++;
	}
}
