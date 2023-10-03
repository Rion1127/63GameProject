#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <array>

struct ConfigSprite {
	std::unique_ptr<Sprite> itemFrameSprite_;
	std::unique_ptr<Sprite> itemtexSprite_;
	std::array<std::unique_ptr<Sprite>, 2> frameSprite_;
	std::array<std::unique_ptr<Sprite>, 2> texSprite_;

	ConfigSprite(Vector2 pos);
	void Update();
	void Draw();
};

class ConfigMenuSprite
{
private:
	
private:
	bool isInversX_;
	bool isInversY_;

	std::unique_ptr<ConfigSprite> axisX_;
	std::unique_ptr<ConfigSprite> axisY_;
public:
	ConfigMenuSprite();
	void Update();
	void Draw();
public:
	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
};

class ConfigMenu
{
private:
	enum class ConfigType {
		CameraAxisX,
		CameraAxisY,
	};
private:
	bool isInversX_;
	bool isInversY_;
	ConfigType type_ = ConfigType::CameraAxisX;

	ConfigMenuSprite sprite_;
private:
	ConfigMenu();
public:
	static ConfigMenu* GetInstance();

	void SpriteUpdate();
	void Update();
	void Draw();
	void DrawImGui();

	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
public:
	bool GetInvX() { return isInversX_; }
	bool GetInvY() { return isInversY_; }
};

