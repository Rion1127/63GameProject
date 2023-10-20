#pragma once
#include "ParticleTrail.h"

/**
 * @file SwordTrail.h
 * @brief 剣の軌道エフェクト
 */

class SwordTrail final :
	public ParticleTrail
{
public:
	SwordTrail(uint32_t vertexSize);
};

