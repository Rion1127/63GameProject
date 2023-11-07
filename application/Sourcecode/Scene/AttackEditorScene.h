#pragma once
#include "IScene.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"

#include "Stage.h"
#include "Player.h"

#include "Spline.h"
#include "Object3d.h"

#include "AttackEditor.h"

/**
 * @file AttackEditorScene.h
 * @brief アタックエディタ用のシーンクラス
 */

class AttackEditorScene :
    public IScene
{
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;

	AttackEditor attackEditor_;
	//床
	std::unique_ptr<Stage> stage_;

public:
	~AttackEditorScene();

	void Ini()override;

	void Update()override;

	void Draw()override;

	void DrawRenderTexture()override;
private:
	void CameraUpdate();

	void LoadEnemyImGui();
};

