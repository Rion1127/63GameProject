#pragma once
#include "IScene.h"
#include "Object3d.h"

class ColDebugScene
	final :
	public IScene
{
private:
	std::unique_ptr<Object3d> col1_;
	std::unique_ptr<Object3d> col2_;


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

