#pragma once
#include "Object3d.h"
#include "DebugCamera.h"
#include "IPostEffect.h"
class PostureDisplay
{
private:
	DebugCamera camera_;
	std::unique_ptr<Object3d> obj_;


public:
	PostureDisplay();

	void Update(Quaternion q);

	void Draw();
	void DrawImGui();
};

