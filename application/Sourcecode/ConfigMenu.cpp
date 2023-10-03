#include "ConfigMenu.h"
#include <imgui.h>
#include "mInput.h"

ConfigMenu* ConfigMenu::GetInstance()
{
	static ConfigMenu instance;
	return &instance;
}

void ConfigMenu::SpriteUpdate()
{
	sprite_.Update();
}

void ConfigMenu::Update()
{
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetTriggerButtons(PAD::INPUT_LEFT))
	{
		//type_ = 
	}


	if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT)||
		Controller::GetTriggerButtons(PAD::INPUT_LEFT))
	{
		if (type_ == ConfigType::CameraAxisX)
		{
			isInversX_ = (isInversX_ == false) ? true : false;
		}
		else if(type_ == ConfigType::CameraAxisY)
		{
			isInversY_ = (isInversY_ == false) ? true : false;
		}
	}
	sprite_.SetInvX(isInversX_);
	sprite_.SetInvY(isInversY_);
}

void ConfigMenu::Draw()
{
	sprite_.Draw();
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
ConfigMenuSprite::ConfigMenuSprite()
{
	axisX_ = std::make_unique<ConfigSprite>(Vector2(0,0));
	axisY_ = std::make_unique<ConfigSprite>(Vector2(0,60));
}
void ConfigMenuSprite::Update()
{
	Color selectColor = { 230,50,50,255 };
	Color unSelectColor = { 0,35,255,255 };
	for (uint32_t i = 0; i < 2; i++)
	{
		axisX_->frameSprite_[i]->SetColor(unSelectColor);
		axisX_->frameSprite_[isInversX_]->SetColor(selectColor);

		axisX_->frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisX_->frameSprite_[isInversX_]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));

		axisY_->frameSprite_[i]->SetColor(unSelectColor);
		axisY_->frameSprite_[isInversY_]->SetColor(selectColor);

		axisY_->frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisY_->frameSprite_[isInversY_]->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
	}
	axisX_->Update();
	axisY_->Update();
}

void ConfigMenuSprite::Draw()
{
	axisX_->Draw();
	axisY_->Draw();
}
#pragma endregion

#pragma region ConfigMenuSprite
ConfigSprite::ConfigSprite(Vector2 pos)
{
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i] = std::make_unique<Sprite>();
		texSprite_[i] = std::make_unique<Sprite>();

		frameSprite_[i]->Ini();
		texSprite_[i]->Ini();

		frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		Vector2 framePos = {
			pos.x + WinAPI::GetWindowSize().x / 2.f + 200.f * i,
			pos.y + 450.f
		};
		frameSprite_[i]->SetPos(framePos);
		frameSprite_[i]->SetScale(Vector2(0.5f, 0.6f));
		texSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("CameraInvTex"));
		Vector2 texPos = {
			pos.x + WinAPI::GetWindowSize().x / 2.f + 200.f * i + (10 * i),
			pos.y + 450.f
		};
		
		float leftUpIndex = 0;
		if (i == 0) leftUpIndex = 0;
		if (i == 1) leftUpIndex = 1;
		Vector2 leftTopPos = {
			200.f * leftUpIndex,
			0
		};
		Vector2 texSize = {
			200.f,
			38
		};
		Vector2 scale = {
			(1.f / 2.f) * 0.6f,
			0.6f
		};
		texSprite_[i]->SetPos(texPos);
		texSprite_[i]->SetTex_LeftTop(leftTopPos);
		texSprite_[i]->SetTex_Size(texSize);
		texSprite_[i]->SetScale(scale);

		frameSprite_[i]->Update();
		texSprite_[i]->Update();
	}

	itemtexSprite_ = std::make_unique<Sprite>();
	itemFrameSprite_ = std::make_unique<Sprite>();

	itemtexSprite_->Ini();
	itemFrameSprite_->Ini();

	itemtexSprite_->Update();
	itemFrameSprite_->Update();
}
void ConfigSprite::Update()
{
	itemFrameSprite_->Update();
	itemtexSprite_->Update();
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i]->Update();
		texSprite_[i]->Update();
	}
}
void ConfigSprite::Draw()
{
	itemFrameSprite_->Draw();
	itemtexSprite_->Draw();
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i]->Draw();
		texSprite_[i]->Draw();
	}
}
#pragma endregion