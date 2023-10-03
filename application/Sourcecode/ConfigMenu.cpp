#include "ConfigMenu.h"
#include <imgui.h>

ConfigMenu* ConfigMenu::GetInstance()
{
	static ConfigMenu instance;
	return &instance;
}

void ConfigMenu::DrawImGui()
{
	ImGui::Begin("Config");

	if (ImGui::Button("InvX"))
	{

		isInversX_ = (isInversX_ == false) ? true : false;
	}
	
	

	if (ImGui::Button("InvY"))
	{
		isInversY_ = (isInversY_ == false) ? true : false;
	}

	ImGui::End();
}

ConfigMenu::ConfigMenu() {
	isInversX_ = false;
	isInversY_ = false;
}

#pragma region ConfigMenuSprite
void ConfigMenuSprite::Update()
{
}

void ConfigMenuSprite::Draw()
{
}
#pragma endregion