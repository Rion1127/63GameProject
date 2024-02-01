#include "ComboCounter.h"
#include "Util.h"

int32_t ComboCounter::comboCount_;
TimerFloat ComboCounter::chainTimer_;
const float ComboCounter::timerLimit_ = 120;
const float ComboCounter::attenValue_ = 30;

#pragma region
void ComboCounter::Update()
{
}

void ComboCounter::AddCombo()
{
	//コンボが長く続いていたら待ち受け時間を短くする
	float rate = comboCount_ - attenValue_;
	rate = Max(rate, 0.f);
	float limitTime = timerLimit_ - (attenValue_ * rate);
	chainTimer_.SetLimitTime(limitTime);
}
void ComboCounter::Draw()
{
}
#pragma endregion

#pragma region
ComboSprite::ComboSprite()
{
}

void ComboSprite::Update()
{
}

void ComboSprite::Draw()
{
}
void ComboSprite::ReactionInit()
{
}
#pragma endregion
