#include "HitStop.h"

void HitStop::HitStopUpdate()
{
	timer_.AddTime(1);
}

void HitStop::SetHitStopTimer(float timer)
{
	timer_.SetLimitTime(timer);
	timer_.Reset();
}
