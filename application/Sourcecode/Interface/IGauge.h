#pragma once
#include "Sprite.h"

/**
 * @file IGauge.h
 * @brief インターフェースクラス
 * @brief ゲージを作る際に継承するクラス
 */

class IGauge
{
protected:
	std::unique_ptr<Sprite> gauge_;
	std::unique_ptr<Sprite> gaugeFrame_;

	Vector2 offsetPos_;
	Vector2 offsetRate_;
	Vector2 pos_;
	Vector2 scale_;

	int32_t maxValue_;
	int32_t nowValue_;
public:
	IGauge(const Color& color);
	void Update(int32_t maxHp, int32_t nowHp);
	virtual void OriginalUpdate() = 0;
	virtual void OriginalFrontDraw() = 0;
	virtual void OriginalMiddleDraw() = 0;
	virtual void OriginalBackDraw() = 0;
	void Draw();
};

