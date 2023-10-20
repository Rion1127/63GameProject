#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <array>
#include "Particle2DSelect.h"

/**
 * @file ConfigMenu.h
 * @brief ゲーム内の設定を管理するクラス
 * @brief SE・BGNとカメラの左右上下反転等
 */

enum class ConfigType {
	BGM,
	SE,
	CameraAxisX,
	CameraAxisY,

	ConfigTypeEND
};
#pragma region CameraConfig
struct ConfigSprite {
	std::unique_ptr<Sprite> itemFrameSprite_;
	std::unique_ptr<Sprite> itemtexSprite_;
	std::array<std::unique_ptr<Sprite>, 2> frameSprite_;
	std::array<std::unique_ptr<Sprite>, 2> texSprite_;

	ConfigSprite(const Vector2& pos, int32_t itemIndex);
	void Update();
	void Draw();
};
#pragma endregion

#pragma region Sound
struct SoundConfigSprite {
	std::unique_ptr<Sprite> itemFrameSprite_;
	std::unique_ptr<Sprite> itemtexSprite_;
	std::array<std::unique_ptr<Sprite>, 10> frameSprite_;
	
	SoundConfigSprite(const Vector2& pos, int32_t itemIndex);
	void Update();
	void Draw();
};
#pragma endregion

class ConfigMenuSprite
{
private:

private:
	Vector2 particlePos;

	bool isInversX_;
	bool isInversY_;
	int32_t bgmVolume_;
	int32_t seVolume_;

	ConfigType type_ = ConfigType::CameraAxisX;

	Particle2DSelect selectParticle_;

	std::unique_ptr<ConfigSprite> axisX_;
	std::unique_ptr<ConfigSprite> axisY_;

	std::unique_ptr<SoundConfigSprite> bgmConfig_;
	std::unique_ptr<SoundConfigSprite> seConfig_;
public:
	ConfigMenuSprite();
	void Update();
	void Draw();
public:
	void SetInvX(bool flag) { isInversX_ = flag; }
	void SetInvY(bool flag) { isInversY_ = flag; }
	void SetBgmVolume(int32_t volume) { bgmVolume_ = volume; }
	void SetSeVolume(int32_t volume) { seVolume_ = volume; }
	void SetConfigType(ConfigType type) { type_ = type; }
private:
	void CameraConfigUpdate();
	void SoundVolumeUpdate();
	void SpriteColorUpdate();
	
};

class ConfigMenu
{
private:

private:
	bool isInversX_;
	bool isInversY_;
	int32_t bgmVolume_;
	int32_t seVolume_;
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

