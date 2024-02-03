#pragma once
#include "Timer.h"
#include "Sprite.h"
#include <array>

class ComboSprite {
private:
	std::array<Sprite, 3> numSprite_;
	Sprite gaugeFrameSprite_;
	Sprite gaugeBackSprite_;
	Sprite gaugeSprite_;
	
	int32_t comboNum_;
	float spriteInterval_;
	Vector2 baseScale_;
	Vector2 basePos_;
	Vector2 gaugePos_;
	float scaleSubSpeed_;
	float reactionScale_;
	int32_t digitNum_;
	Color mainColor_;
	Color gaugeColor_;
	Color gaugeInitColor_;
public:
	ComboSprite();
	void Update(float rate);
	void Draw();
public:
	void SetComboNum(int32_t num);
private:
	//コンボが増えた時のリアクション
	void ReactionInit();
};

class ComboCounter
{
private:
	int32_t comboCount_;
	const int32_t maxComboNum_ = 999;
	const float timerLimit_ = 180;
	const float attenValue_ = 20;
	TimerFloat chainTimer_;
	ComboSprite comboSprite_;
private:
	ComboCounter();
public:
	static ComboCounter* GetInstance();
	//更新
	void Update();
	//コンボ加算
	void AddCombo();
	//コンボを0にする
	void ResetCombo();
	//描画
	void Draw();
};