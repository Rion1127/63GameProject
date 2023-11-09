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
		0.945f,
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

#pragma region MPゲージ
PlayerMPGauge::PlayerMPGauge() :
	IGauge(Color(0.f, 80.f, 255.f, 255.f))
{
	gauge_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));
	gaugeFrame_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));

	offsetRate_ = {
		0.9875f,
		0.9f,
	};
	pos_ = {
		WinAPI::GetWindowSize().x * offsetRate_.x,
		WinAPI::GetWindowSize().y * offsetRate_.y
	};
	basePos_ = pos_;
	easeStartPos_ = basePos_;
	easeStartPos_.x = WinAPI::GetWindowSize().x * 1.5f;
	scale_ = { 1.0f,0.2f };

	gauge_->SetAnchor({ 1,0 });
	gaugeFrame_->SetAnchor({ 1,0 });

	gauge_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	gauge_->SetScale(scale_);

	gaugeFrame_->SetColor({ 255.f,255.f,255.f,255.f });
	gaugeFrame_->SetPos(pos_);
	gaugeFrame_->SetScale(scale_);

	easeTimer_.SetLimitTime(40);
}

void PlayerMPGauge::OriginalUpdate()
{
	easeTimer_.AddTime(1);

	Vector2 pos = {
			Easing::Back::easeOut(easeStartPos_.x,basePos_.x,easeTimer_.GetTimeRate()),
			basePos_.y
	};

	pos_ = pos;

	gauge_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	gaugeFrame_->SetPos(pos_);

	//MPチャージ中
	if (isCharge_ == true) {
		if (easeTimer_.GetIsEnd()) {

		}
		gauge_->SetColor(Color(200.f, 0.f, 170.f, 255.f));
	}
	//通常時
	else {
		gauge_->SetColor(Color(0.f, 80.f, 255.f, 255.f));
	}
}

void PlayerMPGauge::OriginalFrontDraw()
{
	//gaugeFrame_->DrawImGui();
	//gauge_->DrawImGui();
}
#pragma endregion 