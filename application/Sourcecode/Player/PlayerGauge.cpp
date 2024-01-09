#include "PlayerGauge.h"
#include "Easing.h"

/**
 * @file PlayerGauge.cpp
 * @brief HP・MPゲージを描画するクラス
 */

#pragma region HPゲージ
PlayerHPGauge::PlayerHPGauge() :
	IGauge(Color(50.f, 205.f, 50.f, 255.f))
{

	hpBarMiddle_ = std::move(std::make_unique<Sprite>());

	hpBarMiddle_->Ini("");
	gauge_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));
	gaugeFrame_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));
	hpBarMiddle_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));

	gauge_->SetAnchor({ 1,0 });
	gaugeFrame_->SetAnchor({ 1,0 });
	hpBarMiddle_->SetAnchor({ 1,0 });
	offsetPos_ = {
		16.f,32.f
	};

	offsetRate_ = {
		0.9875f,
		0.93f,
	};
	pos_ = {
		WinAPI::GetWindowSize().x * offsetRate_.x,
		WinAPI::GetWindowSize().y * offsetRate_.y
	};
	scale_ = { 1.0f,0.3f };

	gauge_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	gauge_->SetScale(scale_);

	hpBarMiddle_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	hpBarMiddle_->SetColor({ 240.f,40.f,40.f,255.f });
	hpBarMiddle_->SetScale(scale_);

	gaugeFrame_->SetColor({ 255.f,255.f,255.f,255.f });
	gaugeFrame_->SetPos(pos_);

	scale_ = { 1.0f,0.32f };
	gaugeFrame_->SetScale(scale_);
}

void PlayerHPGauge::OriginalUpdate()
{
	Color color = hpBarMiddle_->GetColor();
	color.a -= 1.5f;
	if (color.a <= 0)
	{
		hpBarMiddle_->SetScale(gauge_->GetScale());
	}
	hpBarMiddle_->SetColor(color);
	hpBarMiddle_->Update();
}

void PlayerHPGauge::OriginalMiddleDraw()
{
	hpBarMiddle_->Draw();
}

void PlayerHPGauge::Damage()
{
	Color color = hpBarMiddle_->GetColor();
	color.a = 255.f;
	hpBarMiddle_->SetColor(color);
	hpBarMiddle_->SetScale(gauge_->GetScale());
}
#pragma endregion
