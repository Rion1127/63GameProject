#pragma once
#include "Sprite.h"
#include "IGauge.h"
#include "Timer.h"
class PlayerHPGauge : 
	public IGauge
{
private:
	std::unique_ptr<Sprite> hpBarMiddle_;	//ダメージが減った分を赤く表示する
public:
	PlayerHPGauge();
	
	void Damage();
private:
	void OriginalUpdate() override;
	void OriginalFrontDraw() override {};
	void OriginalMiddleDraw()override;
	void OriginalBackDraw()override {};
};

class PlayerMPGauge :
	public IGauge
{
private:
	Timer easeTimer_;
	Vector2 basePos_;
	//画面外のポジション
	Vector2 easeStartPos_;

	bool isCharge_;
public:
	PlayerMPGauge();

private:
	void OriginalUpdate() override;
	void OriginalFrontDraw() override;
	void OriginalMiddleDraw()override {};
	void OriginalBackDraw()override {};

public:
	void SetIsCharge(bool flag) { isCharge_ = flag; easeTimer_.Reset(); }
public:
	bool GetIsCharge() { return isCharge_; }
};

