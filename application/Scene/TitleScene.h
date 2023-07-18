#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include "LightManager.h"

#include "Floor.h"
#include "Player.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "EnemyManager.h"

class TitleScene
	final :
	public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	

	std::unique_ptr<Sprite> pressASprite_;

public:
	~TitleScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	void CameraUpdate();
};

