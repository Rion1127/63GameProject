#pragma once
#include "Sprite.h"

class ConfigMenuSprite
{
private:
	bool isInversX_;
	bool isInversY_;
private:
	ConfigMenuSprite();
public:
	void Update();
	void Draw();
public:
	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
};

class ConfigMenu
{
private:
	bool isInversX_;
	bool isInversY_;
private:
	ConfigMenu();
public:
	static ConfigMenu* GetInstance();

	void DrawImGui();

	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
public:
	bool GetInvX() { return isInversX_; }
	bool GetInvY() { return isInversY_; }
};

