#include "AttackEffectEditor.h"
#include <imgui.h>
#include "Util.h"
#include <fstream>
#include <iostream>

/**
 * @file AttackEffectEditor.cpp
 * @brief アタックエディターの演出に関連する設定をするクラス
 */

AttackEffectEditor::AttackEffectEditor()
{
}

void AttackEffectEditor::Update()
{
	if (isDelete_) {
		isDelete_ = false;
		effectInfo_.erase(effectInfo_.begin() + effectInfo_.size() - 1);
	}
}

void AttackEffectEditor::Draw()
{
}

void AttackEffectEditor::DrawImGui()
{
	ImGui::Begin("EffectEditor");
	//追加
	ImGuiAddInfo();
	ImGui::SameLine();
	//削除
	ImGuiDeleteInfo();
	//詳細を変更
	ImGuiInfoEdit();

	ImGui::End();
}

void AttackEffectEditor::OutPutStatus(std::ofstream& writing_file)
{
	if (effectInfo_.size() <= 0)return;
	std::string writing_text = "//--EffectInfo--//";
	int32_t index = 0;
	for (auto& info : effectInfo_) {
		std::string tagName = "EffectInfo" + NumberToString(index);
		
		writing_file << writing_text << std::endl;
		writing_text = "Frame";
		writing_file << writing_text << " " << info.frame << std::endl;
		writing_text = "cameraShakeTime";
		writing_file << writing_text << " " << info.cameraShakeTime << std::endl;
		writing_text = "cameraShakePower";
		writing_file << writing_text << " " << info.cameraShakePower << std::endl;
		writing_text = "particleName";
		writing_file << writing_text << " " << info.particleName.c_str() << std::endl;

		index++;
	}
}

void AttackEffectEditor::InPutStatus(const std::string& key, std::stringstream& line_stream)
{
	auto& info = effectInfo_;
	if (key == "//--AtatckInfo--//")
	{
		effectInfo_.clear();
	}
	if (key.find("EffectInfo") != std::string::npos) {
		effectInfo_.emplace_back();
	}
	if (info.size() < 1) return;;
	LoadFileString(line_stream, key, "Frame", info.back().frame);
	LoadFileString(line_stream, key, "cameraShakeTime", info.back().cameraShakeTime);
	LoadFileString(line_stream, key, "cameraShakePower", info.back().cameraShakePower);
	LoadFileString(line_stream, key, "particleName", info.back().particleName);
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
			ImGui::InputText("particleName", info.particleName.data(), 20);
		}
		index++;
	}
}

void AttackEffectEditor::ImGuiAddInfo()
{
	if (ImGui::Button("Add", ImVec2(50, 50))) {
		effectInfo_.emplace_back();
	}
}

void AttackEffectEditor::ImGuiDeleteInfo()
{
	if (ImGui::Button("Delete", ImVec2(50, 50))) {
		isDelete_ = true;
	}
}
