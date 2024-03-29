#pragma once
#include "Sprite.h"
#include "DirectX.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include <sstream>
#include <fstream>
#include "LightManager.h"
#include "Object3d.h"
#include "RRandom.h"

#include "AssimpObject3D.h"
#include "AssimpLoader.h"

#include "Floor.h"
#include "Player.h"
#include "GameCamera.h"
#include "CollisionManager.h"
#include "EnemyManager.h"

class GameScene final:
	public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;
	
	GameCamera gameCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;

	//��
	std::unique_ptr<Floor> floor_;
	//�v���C���[
	std::unique_ptr<Player> player_;

	std::unique_ptr<CollisionManager> colManager_;
	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<AssimpModel> testModel_;

	AssimpObject3D assimpObj_;
public:
	~GameScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	void CameraUpdate();
};

