#pragma once
#include "Object3d.h"
#include "Collision.h"
class IActor
{
protected:
	//���f��
	std::unique_ptr<Object3d> obj_;
	Sphere col_;
public:
	Sphere GetCol() { return col_; }
	WorldTransform* GetWorldTransform() { return obj_->GetTransform(); }
};

