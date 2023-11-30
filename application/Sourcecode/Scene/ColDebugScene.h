#pragma once
#include "IScene.h"
#include "Object3d.h"
#include "Collision.h"
#include "LightManager.h"
#include "DebugCamera.h"

class ColDebugScene
	final :
	public IScene
{
private:
	std::shared_ptr<LightManager> lightManager_ = nullptr;
	std::unique_ptr<Object3d> col1Obj_;
	std::unique_ptr<Object3d> col2Obj_;

	Sphere col1_;
	Sphere col2_;

	DebugCamera debugCamera_;
public:
	~ColDebugScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

private:
	void CameraUpdate();
};

