#include "SwordTrail.h"
#include "Texture.h"

SwordTrail::SwordTrail(uint32_t vertexSize) :
	ParticleTrail(vertexSize)
{
	texture_ = *TextureManager::GetInstance()->GetTexture("SwordTrail");
}
