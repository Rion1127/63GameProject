#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"
#include "Object3d.h"

/**
 * @file GameOverScene.h
 * @brief ゲームオーバーシーンクラス
 */

class GameOverScene final :
	public IScene
{
private:
	enum class SelectType {
		Continue,
		Title
	};
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	std::unique_ptr<Object3d> playerObj_;


	std::unique_ptr<Sprite> titleSprite_;
	std::unique_ptr<Sprite> continueSprite_;
	std::unique_ptr<Sprite> backSprite_;

	SelectType selectType_;
public:
	~GameOverScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
	void DrawPostEffect()override;
private:
	void CameraUpdate();
};
