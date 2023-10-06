#include "IGauge.h"


IGauge::IGauge(Color color)
{
	gauge_ = std::move(std::make_unique<Sprite>());
	gaugeFrame_ = std::move(std::make_unique<Sprite>());

	gauge_->Ini();
	gaugeFrame_->Ini();

	gauge_->SetColor(color);
}

void IGauge::Update(int32_t maxHp, int32_t nowHp)
{
	float scaleRate = (float)nowHp / maxHp;

	gauge_->SetScale({ scaleRate,gauge_->GetScale().y });
	//継承先の更新
	OriginalUpdate();

	gauge_->Update();
	gaugeFrame_->Update();
}

void IGauge::Draw()
{
	OriginalBackDraw();

	gaugeFrame_->Draw();

	OriginalMiddleDraw();

	gauge_->Draw();

	OriginalFrontDraw();
}