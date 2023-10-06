#pragma once
#include <stdint.h>

class Timer {
private:
	int32_t timer_;

	int32_t limitTime_;

	bool isEnd_;
	bool isLoop_;
public:
	Timer();

	//カウントアップ
	void AddTime(int32_t addValue);
	//カウントダウン
	void SubTime(int32_t addValue);
	void Reset();
public:
	//セッター
	void SetLimitTime(int32_t limitTime) { limitTime_ = limitTime; }
	void SetTime(int32_t setValue) { timer_ = setValue; }
	void SetIsEnd(bool flag) { isEnd_ = flag; }
	void SetIsLoop(bool flag) { isLoop_ = flag; }
public:
	//ゲッター
	int32_t GetTimer() { return timer_; }
	int32_t GetLimitTimer() { return limitTime_; }
	bool GetIsEnd() { return isEnd_; }
	float GetTimeRate() { return (float)timer_ / limitTime_; }
};

class TimerFloat {
private:
	float timer_;

	float limitTime_;

	bool isEnd_;
	bool isLoop_;
public:
	TimerFloat();

	//カウントアップ
	void AddTime(float addValue);
	//カウントダウン
	void SubTime(float addValue);
	void Reset();
public:
	//セッター
	void SetLimitTime(float limitTime) { limitTime_ = limitTime; }
	void SetTime(float setValue) { timer_ = setValue; }
	void SetIsEnd(bool flag) { isEnd_ = flag; }
	void SetIsLoop(bool flag) { isLoop_ = flag; }
public:
	//ゲッター
	float GetTimer() { return timer_; }
	float GetLimitTimer() { return limitTime_; }
	bool GetIsEnd() { return isEnd_; }
	float GetTimeRate() { return (float)timer_ / limitTime_; }
};