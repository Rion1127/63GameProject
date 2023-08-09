#pragma once
#include "Sprite.h"
class PauseMenu
{
private:
	enum SelectType {
		Continue,
		Title
	};
private:
	std::unique_ptr<Sprite> backSprite_;
	std::unique_ptr<Sprite> pauseSprite_;
	std::unique_ptr<Sprite> continueSprite_;
	std::unique_ptr<Sprite> titleSprite_;

	SelectType selectType_;
	bool isPause_;
public:
	PauseMenu();
	void Update();
	void Draw();
public:
	bool GetIsPause() { return isPause_; }
};

