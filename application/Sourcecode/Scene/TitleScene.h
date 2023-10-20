#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"

/**
 * @file TitleScene.h
 * @brief タイトルを管理するクラス
 */

class TitleScene
	final :
	public IScene
{
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	std::unique_ptr<Sprite> titleSprite_;
	std::unique_ptr<Sprite> backSprite_;

	std::unique_ptr<Sprite> pressASprite_;

public:
	~TitleScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
	void DrawPostEffect()override;

private:
	void CameraUpdate();
};

