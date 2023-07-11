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
		//timer_‚ªendTime_‚Æ“¯‚¶’l‚É‚È‚Á‚½‚ç
		if (timer_ >= limitTime_) isEnd_ = true;
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
		//timer_‚ªendTime_‚Æ“¯‚¶’l‚É‚È‚Á‚½‚ç
		if (timer_ >= limitTime_) isEnd_ = true;
	}
}

void Timer::Reset()
{
	timer_ = 0;
	isEnd_ = false;
}
