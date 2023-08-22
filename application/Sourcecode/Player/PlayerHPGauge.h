#pragma once
#include "Sprite.h"
#include "IGauge.h"
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

