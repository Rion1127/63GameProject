#pragma once
#include "Collision.h"
#include "Object3d.h"
#include <unordered_map>

/**
 * @file Stage.h
 * @brief 背景のステージを描画するクラス
 */

class Stage
{
private:
	enum Col {
		Back,
		Front,
		Right,
		Left,
		Floor,
	};
private:
	std::unordered_map<std::string, std::unique_ptr<Object3d>> stageObj_;

	std::array<std::unique_ptr<Plane>, 4> walls_;
	std::unique_ptr<Plane> floor_;
public:
	Stage();

	void Update();

	void Draw();
public:
	std::array<std::unique_ptr<Plane>, 4>* GetPlaneCol() { return &walls_; }
	Plane* GetFloor() { return floor_.get(); }
	std::unordered_map<std::string, std::unique_ptr<Object3d>>* GetObjects() { return &stageObj_; }
};

