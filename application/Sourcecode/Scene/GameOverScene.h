#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"
#include "Object3d.h"
#include "Timer.h"

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

	//タイトルへ戻る
	std::unique_ptr<Sprite> titleSprite_;
	std::unique_ptr<Sprite> titleBackSprite_;
	//リトライ
	std::unique_ptr<Sprite> continueSprite_;
	std::unique_ptr<Sprite> continueBackSprite_;
	//黒背景
	std::unique_ptr<Sprite> backSprite_;

	SelectType selectType_;
	Camera camera_;
	TimerFloat floatTimer_;
public:
	~GameOverScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;
private:
	void CameraUpdate();
	void SpriteInit();
};