#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"

#include "Stage.h"
#include "Player.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "UIOperation.h"
#include "ColosseumSystem.h"
#include "PauseMenu.h"

#include "Spline.h"
#include "Object3d.h"
#include "AssimpObject3D.h"
#include "AssimpLoader.h"
class DebugScene final :
	public IScene
{
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	GameCamera gameCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;

	//床
	std::unique_ptr<Stage> stage_;
	//プレイヤー
	std::unique_ptr<Player> player_;

	std::unique_ptr<CollisionManager> colManager_;
	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<UIOperation> operationUI_;
	std::unique_ptr<ColosseumSystem> colosseumSystem_;
	std::unique_ptr<PauseMenu> pauseMenu_;

	std::shared_ptr<ContinuousEmitter> fireEmitter_;

	AssimpObject3D obj_;
public:
	~DebugScene();

	void Ini()override;

	void Update()override;

	void Draw()override;

	void DrawPostEffect()override;
private:
	void CameraUpdate();

	void LoadEnemyImGui();
};

