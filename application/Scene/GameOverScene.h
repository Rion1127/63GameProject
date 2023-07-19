#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include "LightManager.h"

class GameOverScene final :
	public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;



	std::unique_ptr<Sprite> pressASprite_;

	std::unique_ptr<Sprite> titleSprite_;
	std::unique_ptr<Sprite> retrySprite_;

public:
	~GameOverScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	void CameraUpdate();
};
