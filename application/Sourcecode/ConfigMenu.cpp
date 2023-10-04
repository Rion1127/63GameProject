#include "ConfigMenu.h"
#include <imgui.h>
#include "mInput.h"
#include "mSound.h"

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
		Controller::GetTriggerButtons(PAD::INPUT_UP))
	{
		SoundManager::Play("SelectSE");
		int32_t typeNum = (int32_t)type_;
		
		if (Controller::GetTriggerButtons(PAD::INPUT_DOWN))typeNum++;
		if (Controller::GetTriggerButtons(PAD::INPUT_UP))typeNum--;
		typeNum = Clamp(typeNum,0,(int32_t)ConfigType::ConfigTypeEND - 1);

		type_ = (ConfigType)typeNum;

		sprite_.SetConfigType(type_);
	}


	if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT)||
		Controller::GetTriggerButtons(PAD::INPUT_LEFT))
	{
		SoundManager::Play("SelectSE");
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
	axisX_ = std::make_unique<ConfigSprite>(Vector2(0,0),0);
	axisY_ = std::make_unique<ConfigSprite>(Vector2(0,60),1);
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

	Vector2 particlePos{};

	if (type_ == ConfigType::CameraAxisX)
	{
		axisX_->itemFrameSprite_->SetColor(selectColor);
		axisY_->itemFrameSprite_->SetColor(unSelectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));

		particlePos = axisX_->itemFrameSprite_->GetPos();

		particlePos = {
			axisX_->itemFrameSprite_->GetPos().x + axisX_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			axisX_->itemFrameSprite_->GetPos().y - axisX_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}
	else if (type_ == ConfigType::CameraAxisY)
	{
		axisX_->itemFrameSprite_->SetColor(unSelectColor);
		axisY_->itemFrameSprite_->SetColor(selectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));

		particlePos = {
			axisY_->itemFrameSprite_->GetPos().x + axisY_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			axisY_->itemFrameSprite_->GetPos().y - axisY_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}

	selectParticle_.SetPos(particlePos);

	axisX_->Update();
	axisY_->Update();
	selectParticle_.Update();
}

void ConfigMenuSprite::Draw()
{
	axisX_->Draw();
	axisY_->Draw();
	selectParticle_.Draw();
}
#pragma endregion

#pragma region ConfigSprite
ConfigSprite::ConfigSprite(Vector2 pos, int32_t itemIndex)
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

	Vector2 texPos = {
			pos.x + WinAPI::GetWindowSize().x / 2.f - 200.f,
			pos.y + 450.f
	};
	Vector2 leftTopPos = {
			80.f * itemIndex,
			0
	};
	Vector2 texSize = {
		80.f,
		38
	};
	Vector2 scale = {
		(1.f / 2.f) * 0.6f,
		0.6f
	};

	itemtexSprite_->SetPos(texPos);
	itemtexSprite_->SetTex_LeftTop(leftTopPos);
	itemtexSprite_->SetTex_Size(texSize);
	itemtexSprite_->SetScale(scale);
	itemtexSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("ConfigCameraTex"));

	itemFrameSprite_->SetPos(texPos);
	itemFrameSprite_->SetScale(Vector2(0.5f, 0.6f));
	itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));

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