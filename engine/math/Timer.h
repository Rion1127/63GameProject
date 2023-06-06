#pragma once
class Timer
{
public:
	//タイマーリセット
	void TimeReset();
	//計測開始
	void TimerStart();
	//更新
	void Update();
	//計測終了
	void TimerStop();
	//現在のタイムを返す
	int GetTime();

private:
	int time;

	int baseTime;
	int nowTime;

	bool isActive;
};

