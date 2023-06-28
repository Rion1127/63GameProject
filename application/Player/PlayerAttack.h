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

	void DrawImGui();
	void DrawCol();

	int GetStep() {return step; }

private:
	Controller* controler_;
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Object3d> model_;
	WorldTransform worldTransform_;

	std::vector<Vector3> attackPoint;

	Sphere col_;

	int step;
	bool isNext_;
	int actionTimer_;
	int maxActionTimer_;
	int nowCount;

	Vector3 UpdateSpline();

	float timeRate;
	int startIndex;
	float startCount = 1;
};

