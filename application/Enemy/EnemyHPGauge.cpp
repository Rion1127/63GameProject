#include "EnemyHPGauge.h"

EnemyHPGauge::EnemyHPGauge()
{
	hpBar_ = std::move(std::make_unique<Sprite>());
	hpBarBack_ = std::move(std::make_unique<Sprite>());

	hpBar_->Ini("hpBar");
	hpBarBack_->Ini("hpBarBack");
	hpBar_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));
	hpBarBack_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));

	hpBar_->SetAnchor({ 1,0 });
	hpBarBack_->SetAnchor({ 1,0 });
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

	hpBar_->SetPos(pos_);
	hpBar_->SetColor({ 0.2f,0.8f,0.2f,1.f });
	hpBarBack_->SetPos(pos_);
	hpBar_->SetScale(scale_);
	hpBarBack_->SetScale(scale_);
}

void EnemyHPGauge::Update()
{
	if (lockOnEnemy_ != nullptr)
	{
		float scaleRate = (float)lockOnEnemy_->GetHealth() / lockOnEnemy_->GetMaxHealth();

		hpBar_->SetScale({ scaleRate,hpBar_->GetScale().y});

		hpBar_->Update();
		hpBarBack_->Update();
	}
}

void EnemyHPGauge::Draw()
{
	if (lockOnEnemy_ != nullptr)
	{
		hpBarBack_->Draw();
		hpBar_->Draw();

		hpBar_->DrawImGui();
		hpBarBack_->DrawImGui();
	}
}
