#include "SwordTrail.h"
#include "Texture.h"

/**
 * @file SwordTrail.cpp
 * @brief 剣の軌道エフェクト
 */

SwordTrail::SwordTrail(uint32_t vertexSize) :
	ParticleTrail(vertexSize)
{
	texture_ = *TextureManager::GetInstance()->GetTexture("SwordTrail");
}
