#pragma once
#include "mInput.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "Camera.h"
#include <memory>
#include <imgui.h>
#include <vector>
#include "Collision.h"
class PlayerAttack
{
public:
	void Init();

	void Update(WorldTransform* playerPos);

private:
	Controller* controler_;
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> model_;
};

