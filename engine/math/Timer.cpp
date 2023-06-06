#include "Util.h"
#include "Timer.h"

void Timer::TimeReset()
{
	time = 0.f;
}

void Timer::TimerStart()
{
	if (isActive == false) {
		isActive = true;
		baseTime = GetNowCount();
	}
}

void Timer::Update()
{
	if (isActive == true) {
		nowTime = GetNowCount();

		time = nowTime - baseTime;
	}
}

void Timer::TimerStop()
{
	isActive = false;
}

int Timer::GetTime()
{
	return time / 1000;
}
