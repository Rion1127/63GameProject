#include "ParticleObjDataPool.h"

ParticleObjDataPool::ParticleObjDataPool()
{
	std::vector<std::string> objName;
	objName.emplace_back("debri");

	for (int32_t i = 0; i < objName.size(); i++) {
		std::unique_ptr<Object3d> newObj;
		newObj->SetModel(Model::CreateOBJ_uniptr(objName[i], true, false));

		particleObj_.insert(std::make_pair(objName[i],std::move(newObj)));
	}
}
