#pragma once
#include "Collision.h"
#include "Object3d.h"
class Floor
{
private:
	std::unique_ptr<Object3d> model_;

	Plane floorCol_;
public:
	Floor();
	void Update();

	void Draw();
public:
	const Plane& GetPlaneCol() { return floorCol_; }

};

