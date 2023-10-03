#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <array>

enum class ConfigType {
	CameraAxisX,
	CameraAxisY,

	ConfigTypeEND
};

struct ConfigSprite {
	std::unique_ptr<Sprite> itemFrameSprite_;
	std::unique_ptr<Sprite> itemtexSprite_;
	std::array<std::unique_ptr<Sprite>, 2> frameSprite_;
	std::array<std::unique_ptr<Sprite>, 2> texSprite_;

	ConfigSprite(Vector2 pos,int32_t itemIndex);
	void Update();
	void Draw();
};

class ConfigMenuSprite
{
private:
	
private:
	bool isInversX_;
	bool isInversY_;

	ConfigType type_ = ConfigType::CameraAxisX;

	std::unique_ptr<ConfigSprite> axisX_;
	std::unique_ptr<ConfigSprite> axisY_;
public:
	ConfigMenuSprite();
	void Update();
	void Draw();
public:
	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
	void SetConfigType(ConfigType type) { type_ = type; }
};

class ConfigMenu
{
private:
	
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

