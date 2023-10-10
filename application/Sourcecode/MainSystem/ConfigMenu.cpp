#include "ConfigMenu.h"
#include <imgui.h>
#include "mInput.h"
#include "mSound.h"
#include "SoundVolume.h"

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
	//上下で設定する項目を変更
	if (Controller::GetTriggerButtons(PAD::INPUT_DOWN) ||
		Controller::GetTriggerButtons(PAD::INPUT_UP))
	{
		int32_t typeNum = (int32_t)type_;

		if (Controller::GetTriggerButtons(PAD::INPUT_DOWN))typeNum++;
		if (Controller::GetTriggerButtons(PAD::INPUT_UP))typeNum--;

		int32_t min = 0;
		int32_t max = (int32_t)ConfigType::ConfigTypeEND - 1;
		typeNum = Clamp(typeNum, min, max);

		type_ = (ConfigType)typeNum;

		sprite_.SetConfigType(type_);
		
		SoundManager::Play("SelectSE",false,SoundVolume::GetValumeSE());
	}

	//左右で選択した項目の設定変更
	if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT) ||
		Controller::GetTriggerButtons(PAD::INPUT_LEFT))
	{
		if (type_ == ConfigType::BGM)
		{
			if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT))bgmVolume_ += 1;
			if (Controller::GetTriggerButtons(PAD::INPUT_LEFT))bgmVolume_ -= 1;

			bgmVolume_ = Clamp(bgmVolume_, 0, 10);

			float bgmVolume = (float)bgmVolume_ / 10.f;

			SoundVolume::SetValumeBGM(bgmVolume);
		}
		else if (type_ == ConfigType::SE)
		{
			if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT))seVolume_ += 1;
			if (Controller::GetTriggerButtons(PAD::INPUT_LEFT))seVolume_ -= 1;

			seVolume_ = Clamp(seVolume_, 0, 10);
			float seVolume = (float)seVolume_ / 10.f;
			SoundVolume::SetValumeSE(seVolume);
		}
		else if (type_ == ConfigType::CameraAxisX)
		{
			isInversX_ = (isInversX_ == false) ? true : false;
		}
		else if (type_ == ConfigType::CameraAxisY)
		{
			isInversY_ = (isInversY_ == false) ? true : false;
		}

		SoundManager::Play("SelectSE", false, SoundVolume::GetValumeSE());
	}
	sprite_.SetInvX(isInversX_);
	sprite_.SetInvY(isInversY_);
	sprite_.SetBgmVolume(bgmVolume_);
	sprite_.SetSeVolume(seVolume_);
}

void ConfigMenu::Draw()
{
	sprite_.Draw();
}

void ConfigMenu::DrawImGui()
{
	ImGui::Begin("Config");

	ImGui::Text("Camera");

	if (ImGui::Button("InvX"))
	{

		isInversX_ = (isInversX_ == false) ? true : false;
	}
	std::string flagX;
	if (isInversX_)flagX = "Normal";
	else flagX = "Invers";
	ImGui::SameLine();
	ImGui::Text(flagX.c_str());

	if (ImGui::Button("InvY"))
	{
		isInversY_ = (isInversY_ == false) ? true : false;
	}
	std::string flagY;
	if (isInversY_)flagY = "Normal";
	else flagY = "Invers";
	ImGui::SameLine();
	ImGui::Text(flagY.c_str());

	ImGui::End();
}

ConfigMenu::ConfigMenu() {
	isInversX_ = false;
	isInversY_ = false;
	bgmVolume_ = 10;
	seVolume_ = 10;
}

#pragma region ConfigMenuSprite
ConfigMenuSprite::ConfigMenuSprite()
{
	axisX_ = std::make_unique<ConfigSprite>(Vector2(0, 0), 0);
	axisY_ = std::make_unique<ConfigSprite>(Vector2(0, 60), 1);

	bgmConfig_ = std::make_unique<SoundConfigSprite>(Vector2(0, 330), 0);
	seConfig_ = std::make_unique<SoundConfigSprite>(Vector2(0, 390), 1);
}
void ConfigMenuSprite::Update()
{
	CameraConfigUpdate();
	SoundVolumeUpdate();

	selectParticle_.SetPos(particlePos);

	axisX_->Update();
	axisY_->Update();
	bgmConfig_->Update();
	seConfig_->Update();
	selectParticle_.Update();
}

void ConfigMenuSprite::Draw()
{
	axisX_->Draw();
	axisY_->Draw();
	bgmConfig_->Draw();
	seConfig_->Draw();
	selectParticle_.Draw();
}

void ConfigMenuSprite::CameraConfigUpdate() {
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

	if (type_ == ConfigType::CameraAxisX)
	{
		particlePos = axisX_->itemFrameSprite_->GetPos();

		particlePos = {
			axisX_->itemFrameSprite_->GetPos().x + axisX_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			axisX_->itemFrameSprite_->GetPos().y - axisX_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}
	else if (type_ == ConfigType::CameraAxisY)
	{
		particlePos = {
			axisY_->itemFrameSprite_->GetPos().x + axisY_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			axisY_->itemFrameSprite_->GetPos().y - axisY_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}

	SpriteColorUpdate();
}

void ConfigMenuSprite::SoundVolumeUpdate() {
	if (type_ == ConfigType::BGM)
	{
		particlePos = bgmConfig_->itemFrameSprite_->GetPos();

		particlePos = {
			bgmConfig_->itemFrameSprite_->GetPos().x + bgmConfig_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			bgmConfig_->itemFrameSprite_->GetPos().y - bgmConfig_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}
	else if (type_ == ConfigType::SE)
	{
		particlePos = {
			seConfig_->itemFrameSprite_->GetPos().x + seConfig_->itemFrameSprite_->GetTexture().size_.x / 4.5f,
			seConfig_->itemFrameSprite_->GetPos().y - seConfig_->itemFrameSprite_->GetTexture().size_.y / 4.f
		};
	}
}

void ConfigMenuSprite::SpriteColorUpdate() {
	Color selectColor = { 230,50,50,255 };
	Color unSelectColor = { 0,35,255,255 };

	if (type_ == ConfigType::CameraAxisX)
	{
		axisX_->itemFrameSprite_->SetColor(selectColor);
		axisY_->itemFrameSprite_->SetColor(unSelectColor);
		bgmConfig_->itemFrameSprite_->SetColor(unSelectColor);
		seConfig_->itemFrameSprite_->SetColor(unSelectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		bgmConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		seConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
	}
	else if (type_ == ConfigType::CameraAxisY)
	{
		axisY_->itemFrameSprite_->SetColor(selectColor);
		axisX_->itemFrameSprite_->SetColor(unSelectColor);
		bgmConfig_->itemFrameSprite_->SetColor(unSelectColor);
		seConfig_->itemFrameSprite_->SetColor(unSelectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
		bgmConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		seConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
	}
	else if (type_ == ConfigType::BGM)
	{
		bgmConfig_->itemFrameSprite_->SetColor(selectColor);
		seConfig_->itemFrameSprite_->SetColor(unSelectColor);
		axisY_->itemFrameSprite_->SetColor(unSelectColor);
		axisX_->itemFrameSprite_->SetColor(unSelectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		bgmConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
		seConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
	}
	else if (type_ == ConfigType::SE)
	{
		seConfig_->itemFrameSprite_->SetColor(selectColor);
		bgmConfig_->itemFrameSprite_->SetColor(unSelectColor);
		axisY_->itemFrameSprite_->SetColor(unSelectColor);
		axisX_->itemFrameSprite_->SetColor(unSelectColor);

		axisX_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		axisY_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		bgmConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));
		seConfig_->itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("SelectFrame"));
	}

	for (int32_t i = 0; i < bgmConfig_->frameSprite_.size(); i++)
	{
		if (i < bgmVolume_)
		{
			bgmConfig_->frameSprite_[i]->SetColor(selectColor);
		}
		else
		{
			bgmConfig_->frameSprite_[i]->SetColor(unSelectColor);
		}

		if (i < seVolume_)
		{
			seConfig_->frameSprite_[i]->SetColor(selectColor);
		}
		else
		{
			seConfig_->frameSprite_[i]->SetColor(unSelectColor);
		}
	}
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

#pragma region SoundConfigSprite
SoundConfigSprite::SoundConfigSprite(Vector2 pos, int32_t itemIndex)
{
	itemtexSprite_ = std::make_unique<Sprite>();
	itemFrameSprite_ = std::make_unique<Sprite>();

	itemtexSprite_->Ini();
	itemFrameSprite_->Ini();

	Vector2 texPos = {
			pos.x + WinAPI::GetWindowSize().x / 2.f - 200.f + 10 * itemIndex,
			pos.y
	};
	Vector2 leftTopPos = {
			120.f * itemIndex,
			0
	};
	Vector2 texSize = {
		120.f,
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
	itemtexSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("BgmSeTex"));

	Vector2 framePos = {
			pos.x + WinAPI::GetWindowSize().x / 2.f - 200.f,
			pos.y
	};
	itemFrameSprite_->SetPos(framePos);
	itemFrameSprite_->SetScale(Vector2(0.5f, 0.6f));
	itemFrameSprite_->SetTexture(TextureManager::GetInstance()->GetTexture("UnselectFrame"));

	itemtexSprite_->Update();
	itemFrameSprite_->Update();

	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i] = std::make_unique<Sprite>();
		frameSprite_[i]->Ini();

		Vector2 soundPos = itemFrameSprite_->GetPos();
		soundPos.x += 128 + 32 * i;

		frameSprite_[i]->SetPos(soundPos);
		frameSprite_[i]->SetScale(Vector2(0.4f,0.4f));
		frameSprite_[i]->SetTexture(TextureManager::GetInstance()->GetTexture("SoundFrame"));
		frameSprite_[i]->Update();
	}
}

void SoundConfigSprite::Update()
{
	itemtexSprite_->Update();
	itemFrameSprite_->Update();

	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i]->Update();
	}
}

void SoundConfigSprite::Draw()
{
	itemFrameSprite_->Draw();
	itemtexSprite_->Draw();
	for (uint32_t i = 0; i < frameSprite_.size(); i++)
	{
		frameSprite_[i]->Draw();
	}
}
#pragma endregion
