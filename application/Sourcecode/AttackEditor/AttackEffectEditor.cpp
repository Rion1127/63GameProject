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

void AttackEffectEditor::DrawImGui(EffectInfo& info)
{
	ImGui::Begin("EffectEditor");
	//追加
	ImGuiAddInfo();
	ImGui::SameLine();
	//削除
	ImGuiDeleteInfo();
	//詳細を変更
	ImGuiInfoEdit(info);

	ImGui::End();
}

void AttackEffectEditor::OutPutStatus(std::ofstream& writing_file, const EffectInfo& info)
{
	std::string writing_text = "//--EffectInfo--//";
	int32_t index = 0;

	std::string tagName = "EffectInfo" + NumberToString(index);

	writing_file << writing_text << std::endl;
	writing_text = "effectFrame";
	writing_file << writing_text << " " << info.frame << std::endl;
	writing_text = "effectcameraShakeTime";
	writing_file << writing_text << " " << info.cameraShakeTime << std::endl;
	writing_text = "effectcameraShakePower";
	writing_file << writing_text << " " << info.cameraShakePower << std::endl;
	writing_text = "effectparticleName";
	writing_file << writing_text << " " << info.particleName.c_str() << std::endl;
	//パーティクルの座標が剣の動きに追従しなければ出力しない
	if (info.isSeparateParticlePos == false)return;
	writing_text = "effectparticlePos";
	writing_file << writing_text << " " << info.separatePos.x << " " << info.separatePos.y <<" " << info.separatePos.z << std::endl;
}

void AttackEffectEditor::InPutStatus(const std::string& key, std::stringstream& line_stream, EffectInfo& info)
{

	if (key == "//--AtatckInfo--//")
	{
		effectInfo_.clear();
	}
	if (key.find("EffectInfo") != std::string::npos) {
		effectInfo_.emplace_back();
	}

	LoadFileString(line_stream, key, "effectFrame", info.frame);
	LoadFileString(line_stream, key, "effectcameraShakeTime", info.cameraShakeTime);
	LoadFileString(line_stream, key, "effectcameraShakePower", info.cameraShakePower);
	LoadFileString(line_stream, key, "effectparticleName", info.particleName);
	if (key == "effectparticlePos")
	{
		info.isSeparateParticlePos = true;
		line_stream >> info.separatePos.x;
		line_stream >> info.separatePos.y;
		line_stream >> info.separatePos.z;
	}
}

void AttackEffectEditor::ImGuiInfoEdit(EffectInfo& info)
{
	int32_t index = 0;
	std::string tagName;

	tagName = "EffectInfo" + NumberToString(index);
	if (ImGui::CollapsingHeader(tagName.c_str()))
	{
		ImGui::DragInt("frame", &info.frame);
		ImGui::DragFloat("cameraShakePower", &info.cameraShakePower, 0.1f);
		ImGui::DragFloat("cameraShakeTime", &info.cameraShakeTime);
		ImGui::InputText("particleName", info.particleName.data(), 20);
		ImGui::Checkbox("isSeparateParticlePos", &info.isSeparateParticlePos);
		if (info.isSeparateParticlePos) {
			float pos[3] = { info.separatePos.x,info.separatePos.y,info.separatePos.z };
			ImGui::DragFloat3("ParticlePos", pos, 0.1f);
			info.separatePos = { pos[0],pos[1],pos[2] };
		}
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
