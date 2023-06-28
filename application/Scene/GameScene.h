#pragma once
#include <memory>
#include "Model.h"
#include "Sprite.h"
#include "Particle.h"
#include "WorldTransform.h"
#include "WinAPI.h"
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

class GameScene :
	public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;
	
	GameCamera gameCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;


	std::unique_ptr<Floor> floor_;

	std::unique_ptr<Player> player_;
public:
	~GameScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
};

