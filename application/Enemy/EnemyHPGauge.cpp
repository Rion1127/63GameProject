#include "EnemyHPGauge.h"

EnemyHPGauge::EnemyHPGauge()
{
	hpBar_ = std::move(std::make_unique<Sprite>());
	hpBarBack_ = std::move(std::make_unique<Sprite>());

	hpBar_->Ini("hpBar");
	hpBarBack_->Ini("hpBarBack");
	hpBarBack_->SetTexture(TextureManager::GetInstance()->GetTexture("HpBarBack"));
}

void EnemyHPGauge::Update()
{
	if (lockOnEnemy_ != nullptr)
	{
		hpBar_->Update();
		hpBarBack_->Update();
	}
}

void EnemyHPGauge::Draw()
{
	if (lockOnEnemy_ != nullptr)
	{
		hpBar_->Draw();
		hpBarBack_->Draw();
		hpBarBack_->DrawImGui();
	}
}
