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

class DebugScene final:
    public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;

	Object3d obj_;
public:
	~DebugScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	void CameraUpdate();
};

