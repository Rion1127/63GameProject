#pragma once
#include "Timer.h"
#include "Sprite.h"
#include <array>
class ComboCounter
{
private:
	static int32_t comboCount_;
	static TimerFloat chainTimer_;
	static const float timerLimit_;
	static const float attenValue_;
public:
	//更新
	static void Update();
	//コンボ加算
	static void AddCombo();
	//描画
	static void Draw();
};

class ComboSprite {
private:
	std::array<Sprite, 2> numSprite_;
	std::array<int, 2> digit_;
	int32_t comboNum_;
public:
	ComboSprite();
	void Update();
	void Draw();
public:
	void SetComboNum(int32_t num) { comboNum_ = num; };
private:
	//コンボが増えた時のリアクション
	void ReactionInit();
};