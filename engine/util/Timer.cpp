#include "Timer.h"
#include "myMath.h"
Timer::Timer()
{
	timer_ = 0;
	limitTime_ = 0;
	isEnd_ = false;
	isLoop_ = false;
}

void Timer::AddTime(int32_t addValue)
{
	timer_ += addValue;

	timer_ = Min(timer_, limitTime_);
	if (isLoop_)
	{
		if (timer_ >= limitTime_) timer_ = 0;
	}
	else
	{
		//timer_がendTime_と同じ値になったら
		if (timer_ >= limitTime_) isEnd_ = true;
		else isEnd_ = false;
	}
}

void Timer::SubTime(int32_t addValue)
{
	timer_ -= addValue;

	timer_ = Max(timer_, limitTime_);
	if (isLoop_)
	{
		if (timer_ >= limitTime_) timer_ = 0;
	}
	else
	{
		//timer_がendTime_と同じ値になったら
		if (timer_ >= limitTime_) isEnd_ = true;
		else isEnd_ = false;
	}
}

void Timer::Reset()
{
	timer_ = 0;
	isEnd_ = false;
}

#pragma region TimerFloat
TimerFloat::TimerFloat()
{
	timer_ = 0;
	limitTime_ = 0;
	isEnd_ = false;
	isLoop_ = false;
}

void TimerFloat::AddTime(float addValue)
{
	timer_ += addValue;

	timer_ = Min(timer_, limitTime_);
	if (isLoop_)
	{
		if (timer_ >= limitTime_) timer_ = 0;
	}
	else
	{
		//timer_がendTime_と同じ値になったら
		if (timer_ >= limitTime_) isEnd_ = true;
		else isEnd_ = false;
	}
}

void TimerFloat::SubTime(float addValue)
{
	timer_ -= addValue;

	timer_ = Max(timer_, limitTime_);
	if (isLoop_)
	{
		if (timer_ >= limitTime_) timer_ = 0;
	}
	else
	{
		//timer_がendTime_と同じ値になったら
		if (timer_ >= limitTime_) isEnd_ = true;
		else isEnd_ = false;
	}
}

void TimerFloat::Reset()
{
	timer_ = 0;
	isEnd_ = false;
}

#pragma endregion