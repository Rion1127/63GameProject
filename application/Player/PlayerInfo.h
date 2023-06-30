#pragma once
#include "WorldTransform.h"
#include "Vector3.h"

struct PlayerInfo
{
	WorldTransform* WT;
	Vector3* frontVec;
};

