#include "PlayerHPGauge.h"


PlayerHPGauge::PlayerHPGauge()
{
	hpBar_ = std::move(std::make_unique<Sprite>());
	hpBarBack_ = std::move(std::make_unique<Sprite>());

	hpBar_->Ini("");
	hpBarBack_->Ini("");
	hpBar_->SetTexture(TextureManager::GetInstance()->GetTexture("Gauge"));
	hpBarBack_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));

	hpBar_->SetAnchor({ 1,0 });
	hpBarBack_->SetAnchor({ 1,0 });
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

	hpBar_->SetPos({ pos_.x - 3.f,pos_.y + 1.5f });
	hpBar_->SetColor({ 50.f,205.f,50.f,255.f });
	hpBar_->SetScale(scale_);

	hpBarBack_->SetColor({ 255.f,255.f,255.f,255.f });
	hpBarBack_->SetPos(pos_);
	hpBarBack_->SetScale(scale_);
}

void PlayerHPGauge::Update(int32_t maxHp, int32_t nowHp)
{
	float scaleRate = (float)nowHp / maxHp;

	hpBar_->SetScale({ scaleRate,hpBar_->GetScale().y });

	hpBar_->Update();
	hpBarBack_->Update();

}

void PlayerHPGauge::Draw()
{
	hpBarBack_->Draw();
	hpBar_->Draw();
}
