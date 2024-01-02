#pragma once
#include "Object3d.h"
#include <unordered_map>
class ParticleObjDataPool
{
private:
	std::unordered_map<std::string, std::unique_ptr<Object3d>> particleObj_;
public:
	ParticleObjDataPool();

	Object3d* GetObj(std::string name) {return particleObj_[name].get(); }
};

