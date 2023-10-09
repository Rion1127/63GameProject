#pragma once
#include "Timer.h"
class HitStop
{
private:
	TimerFloat timer_;
public:
	void HitStopUpdate();
	
	void SetHitStopTimer(float timer);
public:
	//セッター
public:
	//ゲッター
	bool GetIsHitStopping() { return !timer_.GetIsEnd(); }
};

