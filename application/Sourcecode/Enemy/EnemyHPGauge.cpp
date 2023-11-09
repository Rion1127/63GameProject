#include "EnemyHPGauge.h"

/**
 * @file EnemyHPGauge.cpp
 * @brief 敵のHPを表示するクラス
 */

EnemyHPGauge::EnemyHPGauge()
{
	hpBar_ = std::move(std::make_unique<Sprite>());
	hpBarBack_ = std::move(std::make_unique<Sprite>());
	hpBarMiddle_ = std::move(std::make_unique<Sprite>());

	hpBar_->Ini("");
	hpBarBack_->Ini("");
	hpBarMiddle_->Ini("");
	hpBar_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));
	hpBarBack_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));
	hpBarMiddle_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));

	hpBar_->SetAnchor({ 1,0 });
	hpBarBack_->SetAnchor({ 1,0 });
	hpBarMiddle_->SetAnchor({ 1,0 });
	offsetPos_ = {
		16.f,32.f
	};

	offsetRate_ = {
		0.9875f,
		0.045f,
	};
	pos_ = {
		WinAPI::GetWindowSize().x * offsetRate_.x,
		WinAPI::GetWindowSize().y * offsetRate_.y
	};
	scale_ = { 1.0f,0.3f };

	hpBar_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	hpBar_->SetColor({ 50.f,205.f,50.f,255.f });
	hpBar_->SetScale(scale_);

	hpBarMiddle_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	hpBarMiddle_->SetColor({ 240.f,40.f,40.f,255.f });
	hpBarMiddle_->SetScale(scale_);

	hpBarBack_->SetColor({ 255.f,255.f,255.f,255.f });
	hpBarBack_->SetPos(pos_);
	hpBarBack_->SetScale(scale_);
}

void EnemyHPGauge::Update()
{
	if (lockOnEnemy_ != nullptr)
	{
		float scaleRate = (float)lockOnEnemy_->GetHealth() / lockOnEnemy_->GetMaxHealth();

		hpBar_->SetScale({ scaleRate,hpBar_->GetScale().y });

		Color color = hpBarMiddle_->GetColor();
		color.a -= 1.5f;
		if (color.a <= 0)
		{
			hpBarMiddle_->SetScale(hpBar_->GetScale());
		}
		hpBarMiddle_->SetColor(color);

		hpBar_->Update();
		hpBarBack_->Update();
		hpBarMiddle_->Update();
	}
}

void EnemyHPGauge::Draw()
{
	if (lockOnEnemy_ != nullptr)
	{
		hpBarBack_->Draw();
		hpBarMiddle_->Draw();
		hpBar_->Draw();
	}
}

void EnemyHPGauge::Damage()
{
	Color color = hpBarMiddle_->GetColor();
	color.a = 255.f;
	hpBarMiddle_->SetColor(color);
	hpBarMiddle_->SetScale(hpBar_->GetScale());
}
